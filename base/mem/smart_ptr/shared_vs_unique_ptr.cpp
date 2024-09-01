// C++：为什么unique_ptr的Deleter是模板类型参数，而shared_ptr的Deleter不是？

#include <iostream>
#include <memory>

int main() {
  std::shared_ptr<int> psi{new int{}};

  return 0;
}
