#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

#define EFD_NONBLOCK 0x800 // 非阻塞标志
#define MAX_EVENTS 10      // epoll 监听的最大事件数

int efd;  // eventfd 文件描述符
int epfd; // epoll 文件描述符

// 子线程函数：监听 eventfd 事件
void *thread_func(void *arg) {
  struct epoll_event events[MAX_EVENTS];
  uint64_t count;

  while (1) {
    // 等待事件（非阻塞模式）
    int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
    if (n == -1) {
      perror("epoll_wait failed");
      break;
    }

    for (int i = 0; i < n; i++) {
      if (events[i].events & EPOLLIN) {
        // 读取 eventfd 的计数器（清零）
        ssize_t ret = read(events[i].data.fd, &count, sizeof(count));
        if (ret == -1) {
          perror("read failed");
          continue;
        }
        printf("Thread: Received %lu events\n", count);
      }
    }
  }
  return NULL;
}

int main() {
  // 1. 创建 eventfd（初始值 0，非阻塞模式）
  efd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (efd == -1) {
    perror("eventfd creation failed");
    exit(EXIT_FAILURE);
  }

  // 2. 创建 epoll 实例
  epfd = epoll_create1(0);
  if (epfd == -1) {
    perror("epoll_create1 failed");
    close(efd);
    exit(EXIT_FAILURE);
  }

  // 3. 将 eventfd 添加到 epoll 监听列表
  struct epoll_event event;
  event.events = EPOLLIN; // 监听可读事件
  event.data.fd = efd;
  if (epoll_ctl(epfd, EPOLL_CTL_ADD, efd, &event) == -1) {
    perror("epoll_ctl failed");
    close(epfd);
    close(efd);
    exit(EXIT_FAILURE);
  }

  // 4. 创建子线程
  pthread_t thread;
  if (pthread_create(&thread, NULL, thread_func, NULL) != 0) {
    perror("pthread_create failed");
    close(epfd);
    close(efd);
    exit(EXIT_FAILURE);
  }

  // 5. 主线程发送事件（模拟任务通知）
  for (int i = 1; i <= 5; i++) {
    uint64_t event = 1; // 每次写入 1，计数器累加
    ssize_t ret = write(efd, &event, sizeof(event));
    if (ret == -1) {
      perror("write failed");
      break;
    }
    printf("Main: Sent event %d\n", i);
    sleep(1); // 每秒发送一次事件
  }

  // 6. 等待子线程结束并清理资源
  pthread_join(thread, NULL);
  close(epfd);
  close(efd);
  return 0;
}
