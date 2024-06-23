#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>

int main() {
  int fd;
  struct itimerspec new_value, old_value;
  uint64_t expirations;

  // 创建定时器
  fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
  if (fd == -1) {
    perror("timerfd_create");
    exit(EXIT_FAILURE);
  }

  // 设置定时器，5秒后触发
  new_value.it_value.tv_sec = 5;
  new_value.it_value.tv_nsec = 0;
  new_value.it_interval.tv_sec = 0;
  new_value.it_interval.tv_nsec = 0;

  if (timerfd_settime(fd, 0, &new_value, &old_value) == -1) {
    perror("timerfd_settime");
    exit(EXIT_FAILURE);
  }

  // 等待定时器到期
  while (1) {
    if (read(fd, &expirations, sizeof(expirations)) != sizeof(expirations)) {
      perror("read");
      exit(EXIT_FAILURE);
    }
    printf("Timer expired!\n");
    break;
  }

  // 清理资源
  close(fd);

  return 0;
}
