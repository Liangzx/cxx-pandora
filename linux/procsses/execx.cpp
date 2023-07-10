#include <stdio.h>
#include <unistd.h>
extern char **environ;

int func_environ() {
  printf("hello pid=%d\n", getpid());
  int i;
  for (i = 0; environ[i] != NULL; ++i) {
    printf("%s\n", environ[i]);
  }
  return 0;
}

int func_execle() {
  char *const envp[] = {"AA=11", "BB=22", NULL};
  printf("Entering main ...\n");
  int ret;
  // ret =execl("./hello", "hello", NULL);
  ret = execle("./hello", "hello", NULL, envp);
  if (ret == -1) {
    perror("execl error");
  }

  printf("Exiting main ...\n");
  return 0;
}

int main() {
  //   func_environ();
  func_execle();
  return 0;
}