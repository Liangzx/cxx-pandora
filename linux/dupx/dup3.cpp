#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int fd = open("test.txt", O_CREAT | O_RDWR | O_TRUNC, 0774);
  if (fd < 0) {
    perror("open error");
    exit(-1);
  }
  /* 获得fd的初始flags, 然后添加FD_CLOEXEC选项, 最后清除FD_CLOEXEC选项 */
  int flags0 = fcntl(fd, F_GETFD);
  printf("primary flags of fd is %d\n", flags0);
  fcntl(fd, F_SETFD, flags0 | FD_CLOEXEC);
  int flags1 = fcntl(fd, F_GETFD);
  printf("After adding FD_CLOEXEC, flags of fd is %d\n", flags1);
  flags1 = fcntl(fd, F_SETFD, flags0);
  printf("Restore FD_CLOEXEC(cleared), flags of fd is %d\n", flags1);

  int fd2 = dup3(fd, fileno(stderr), O_CLOEXEC); // 2
  if (fd2 < 0) {
    perror("dup error");
    exit(-1);
  }
  printf("fd = %d, fd2 = %d\n", fd, fd2); // fd = 3, fd = 2

  flags1 = fcntl(fd2, F_GETFD);
  printf("flags of fd2 is %d\n", flags1);

  char buf[128];
  ssize_t n;
  /* 从键盘接收输入并写入fd2所指文件 */
  while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
    if (write(fd2, buf, static_cast<size_t>(n)) < 0) {
      printf("Write error!\n");
      exit(-1);
    }
  }
  return 0;
}
