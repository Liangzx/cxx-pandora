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
  
  int fd2 = dup2(fd, fileno(stderr)); // 2
  if (fd2 < 0) {
    perror("dup error");
    exit(-1);
  }

  printf("fd = %d, fd2 = %d\n", fd, fd2); // fd = 3, fd2 = 2
  fprintf(stderr, "test stderr\n");

  char buf[128];
  ssize_t n;
  /* 从键盘接收输入并写入fd2所指文件 */
  int fds[] = {fd, fd2};
  while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {

    for (int i = 0; i < 2; i++) {
      if (write(fds[i], buf, static_cast<size_t>(n)) < 0) {
        printf("Write error!\n");
        exit(-1);
      }
    }
  }
  return 0;
}
