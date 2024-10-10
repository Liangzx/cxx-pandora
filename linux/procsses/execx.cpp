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

void func_exec()
{
  // 执行 /bin/ls 命令，并传递 "-l" 参数
    execlp("lss", "ls", "-l", (char *)0);

    // 如果 exec 失败，才会执行到这里
    perror("execlp");
}

int main() {
  //   func_environ();
  // func_execle();
  func_exec();
  return 0;
}

/**
 * Note:
 * exec 返回值
 * 如果 exec 调用成功，控制权将转移到新程序，不会返回到原来的程序。
 * 如果 exec 调用失败，会返回 -1，并且 errno 会被设置为相应的错误码。
 */
