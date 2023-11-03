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
  int fd2 = dup(fd);
  if (fd2 < 0) {
    perror("dup error");
    exit(-1);
  }
  // 通常fd = 3, fd2 = 4, 因为0,1,2分别为标准输入、标准输出、标准错误
  // 使用lsof 可以看到 fd/fd2 指向同一个文件
  printf("fd = %d, fd2 = %d\n", fd, fd2);

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