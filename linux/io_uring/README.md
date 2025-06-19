# io_uring

## 一、io_uring 的​​原理​​（底层工作机制）

io_uring 的核心思想是：​​通过用户态和内核态共享内存中的环形队列（ring buffer），实现高效的异步 I/O 提交与完成通知，从而避免频繁的系统调用和上下文切换，提高性能。

1. 两个核心队列：SQ（提交队列） 和 CQ（完成队列）

```text
​​Submission Queue (SQ)​​：应用程序往这里面放 I/O 请求（比如“读文件”、“写磁盘”）。
​​Completion Queue (CQ)​​：内核处理完 I/O 请求后，会把结果放到这里，应用程序来取。
这两个队列都是​​环形缓冲区（ring buffer）​​结构，存放在​​用户态和内核态共享的内存区域​​中，避免了频繁的数据拷贝。
```

2. 共享内存机制

```text
io_uring 使用了一块​​用户态和内核态共享的内存区域​​，这个区域包含了：

SQ（提交队列）
CQ（完成队列）
SQEs（Submission Queue Entries，每个 I/O 请求的具体描述）
一些控制信息和状态标志
这样，应用程序和内核可以直接读写这块内存，​​不需要每次都通过系统调用来传递数据​​，极大提升了性能。
```

3. 系统调用接口

```
io_uring 提供了几个关键的系统调用，用于初始化、提交 I/O 请求、等待完成等：
io_uring_setup()	初始化 io_uring，创建共享内存区域
io_uring_enter()	提交 I/O 请求 或 等待完成事件
io_uring_register()	注册文件描述符、缓冲区等资源，减少重复传参

这些系统调用让应用程序可以高效地与内核交互，而不用频繁陷入内核态。
```

4. 事件通知机制

```text
io_uring 支持多种方式来通知应用程序“有 I/O 完成了”：

​​轮询模式（Polling）​​：应用程序不停地检查 CQ 是否有新事件（性能最高，但 CPU 占用高）
​​中断模式（Interrupt）​​：内核在完成 I/O 后通过中断通知应用（更节省 CPU，但延迟略高）
​​结合 epoll 使用​​：可以用 epoll 监听 io_uring 的 fd，实现高效事件驱动模型
```

## 二、io_uring 的​​使用方法​​（如何编程使用）

```text
虽然 io_uring 是 Linux 内核提供的功能，但直接使用系统调用比较复杂。通常我们会使用封装好的库，比如：
​​liburing​​（官方推荐的 C 语言库）
​​其他语言绑定​​（如 Rust 的 tokio-uring）
```

## 使用 io_uring 的基本步骤（以 liburing 为例）

​​1. 初始化 io_uring​

```c
#include <liburing.h>

struct io_uring ring;
int ret = io_uring_queue_init(QUEUE_DEPTH, &ring, 0);
if (ret < 0) {
    // 错误处理
}

/* QUEUE_DEPTH 是队列深度，即最多可以同时提交多少个 I/O 请求。
**/ 这一步会初始化 io_uring，并在用户态和内核态之间建立共享内存区域。
```

2. 准备一个 I/O 请求（比如读文件）

```c
struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
if (!sqe) {
    // 获取 SQE 失败
}

// 设置读操作：从 fd 的 offset 位置读取 len 字节到 buf
io_uring_prep_read(sqe, fd, buf, len, offset);

/*
io_uring_get_sqe() 从提交队列中取出一个空闲的 SQE（Submission Queue Entry）。
io_uring_prep_read() 填充这个 SQE，告诉内核要执行什么操作（这里是“读文件”）。
**/
```

3. 提交 I/O 请求​

```c
ret = io_uring_submit(&ring);
if (ret < 0) {
    // 提交失败
}
// io_uring_submit() 将 SQ 中的请求提交给内核，内核开始处理这些 I/O 操作。
```

4. 等待并处理完成事件​

```c
struct io_uring_cqe *cqe;
ret = io_uring_wait_cqe(&ring, &cqe);
if (ret < 0) {
    // 等待失败
}

// 处理完成事件
if (cqe->res >= 0) {
    // I/O 成功，处理数据
} else {
    // I/O 失败，处理错误
}

// 标记这个 CQE 已经处理完成
io_uring_cqe_seen(&ring, cqe);

/*
io_uring_wait_cqe() 会阻塞等待，直到有 I/O 完成事件出现在 CQ 中。
拿到 cqe 后，可以检查操作是否成功，并获取结果（比如实际读取了多少字节）。
最后调用 io_uring_cqe_seen() 告诉内核这个事件已经处理了，可以复用 CQE。
**/
```

5. 清理资源​

```c
io_uring_queue_exit(&ring);
// 使用完后，释放 io_uring 占用的资源
```

## 示例：使用 io_uring 异步读取文件（C 语言）

1. 环境准备

```shell
# 安装 liburing
sudo apt update
sudo apt install liburing-dev
```

2. 完整代码：async_read.c

```c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <liburing.h>

#define QUEUE_DEPTH 128
#define BUF_SIZE    4096

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *filename = argv[1];
    int fd;
    struct stat st;
    char *buf;
    struct io_uring ring;
    struct io_uring_sqe *sqe;
    struct io_uring_cqe *cqe;
    int ret;

    // 1️⃣ 打开文件
    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 获取文件大小
    if (fstat(fd, &st) < 0) {
        perror("fstat");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // 分配缓冲区
    buf = malloc(BUF_SIZE);
    if (!buf) {
        perror("malloc");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // 2️⃣ 初始化 io_uring
    ret = io_uring_queue_init(QUEUE_DEPTH, &ring, 0);
    if (ret < 0) {
        fprintf(stderr, "io_uring_queue_init failed: %s\n", strerror(-ret));
        free(buf);
        close(fd);
        exit(EXIT_FAILURE);
    }

    // 3️⃣ 准备读操作：从文件 offset=0 开始读取 BUF_SIZE 字节到 buf
    sqe = io_uring_get_sqe(&ring);
    if (!sqe) {
        fprintf(stderr, "io_uring_get_sqe failed\n");
        io_uring_queue_exit(&ring);
        free(buf);
        close(fd);
        exit(EXIT_FAILURE);
    }

    io_uring_prep_read(sqe, fd, buf, BUF_SIZE, 0);  // 从 offset=0 开始读

    // 4️⃣ 提交 I/O 请求到内核
    ret = io_uring_submit(&ring);
    if (ret < 0) {
        fprintf(stderr, "io_uring_submit failed: %s\n", strerror(-ret));
        io_uring_queue_exit(&ring);
        free(buf);
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Submitted read request, waiting for completion...\n");

    // 5️⃣ 等待 I/O 完成事件
    ret = io_uring_wait_cqe(&ring, &cqe);
    if (ret < 0) {
        fprintf(stderr, "io_uring_wait_cqe failed: %s\n", strerror(-ret));
        io_uring_queue_exit(&ring);
        free(buf);
        close(fd);
        exit(EXIT_FAILURE);
    }

    // 6️⃣ 检查 I/O 结果
    if (cqe->res >= 0) {
        printf("Read %d bytes from file:\n", cqe->res);
        write(STDOUT_FILENO, buf, cqe->res);  // 将读取的内容输出到标准输出
    } else {
        fprintf(stderr, "Read failed: %s\n", strerror(-cqe->res));
    }

    // 7️⃣ 标记 CQE 已处理
    io_uring_cqe_seen(&ring, cqe);

    // 8️⃣ 清理资源
    io_uring_queue_exit(&ring);
    free(buf);
    close(fd);

    return 0;
}
```
