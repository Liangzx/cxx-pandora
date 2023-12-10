
#include <complex>
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <tuple>
#include <unistd.h>

int main() {
  std::map<std::string, std::complex<double>> scp;
  scp.emplace(std::piecewise_construct, std::forward_as_tuple("hello"),
              std::forward_as_tuple(1, 2));

  std::cout << scp["hello"] << std::endl;

  return 0;

  int tmp = 0;
  printf("There is fork before\n");
  pid_t res = fork();
  if (res < 0) {
    // fork失败
    perror("fork");
  } else if (res == 0) {
    tmp = 10;
    // 该进程为子进程
    printf("im child[%d],tmp is %d.\n", getpid(), tmp);
  } else {
    // 该进程为父进程
    printf("im father[%d],tmp is %d.\n", getpid(), tmp);
  }
  printf("tmp = %d\n", tmp);
  int status;
  waitpid(-1, &status, 0);
  return 0;
}