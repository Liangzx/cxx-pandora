#include <iostream>
#include <string>
#include <exception>
#include <signal.h>

void handleSegmentationFault(int signal) {
  std::cout << "Segmentation fault." << std::endl;
  /// 使用 exit
  return;
}

int main() {

  // 注册信号处理函数
  signal(SIGSEGV, handleSegmentationFault);
  int *ptr = nullptr;
  *ptr = 10;
  return 0;
}
