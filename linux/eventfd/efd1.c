#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <unistd.h>
#include <errno.h>

int efd; // eventfd 文件描述符
pthread_t thread;

// 线程函数：等待事件并打印计数
void *thread_func(void *arg) {
  uint64_t count;
  while (1) {
    // 阻塞读取（若设置为非阻塞，需处理 EAGAIN 错误）
    ssize_t ret = read(efd, &count, sizeof(count));
    if (ret == -1) {
        if (errno == EAGAIN) {
            // 非阻塞模式下，若没有事件则继续循环
            continue;
        }
      perror("read failed");
      break;
    }
    printf("Thread: Received event, count = %lu\n", count);
  }
  return NULL;
}

int main() {
  // 1. 创建 eventfd（初始值 0，非阻塞模式）
  efd = eventfd(0, EFD_NONBLOCK);
  if (efd == -1) {
    perror("eventfd creation failed");
    exit(EXIT_FAILURE);
  }

  // 2. 创建子线程
  if (pthread_create(&thread, NULL, thread_func, NULL) != 0) {
    perror("pthread_create failed");
    close(efd);
    exit(EXIT_FAILURE);
  }

  // 3. 主线程写入事件（通知子线程）
  for (int i = 1; i <= 5; i++) {
    uint64_t event = 1; // 每次写入 1，计数器会累加
    ssize_t ret = write(efd, &event, sizeof(event));
    if (ret == -1) {
      perror("write failed");
      break;
    }
    printf("Main: Sent event %d\n", i);
    sleep(1); // 每秒发送一次事件
  }

  // 4. 等待线程结束并清理资源
  pthread_join(thread, NULL);
  close(efd);
  return 0;
}
