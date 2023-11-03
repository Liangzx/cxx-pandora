#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int tmp = 3;

int main() {
  pid_t res = vfork();
  if (res < 0) {
    perror("vfork");
    exit(-1);
  } else if (res == 0) {
    tmp = 10;
    printf("child res = %d\n", tmp);
    exit(0);
  } else {
    printf("father res = %d\n", tmp);
  }
  int status;
  wait(&status);
  return 0;
}