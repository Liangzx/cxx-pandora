// https://blog.csdn.net/taiyang1987912/article/details/40651623

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  printf("The real user ID is: %d\n", getuid());
  printf("The effective user ID is :%d\n", geteuid());
  return (0);
}