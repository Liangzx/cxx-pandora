#include <iostream>

class Instance;
using build_ptr = void (Instance::*)(int);
class Instance {
public:
  void Build(std::initializer_list<build_ptr> ptrs) {
    for (auto &p : ptrs) {
      (this->*p)(3);
    }
  }
  void BuildA(int a) { std::cout << "a " << a << std::endl; };
  void BuildB(int b) { std::cout << "b " << b << std::endl; };
  void static BuildC(int c) {std::cout << "c " << c << std::endl; };
};

int main() {
  Instance ins;
  build_ptr pp = &Instance::BuildA;
  (ins.*pp)(12);
  ins.Build({&Instance::BuildA, &Instance::BuildB});
  // 16
  std::cout << sizeof(&Instance::BuildA) << std::endl;
  // 8
  std::cout << sizeof(&Instance::BuildC) << std::endl;
  std::cout << std::boolalpha;
  // false
  std::cout << std::is_pointer<decltype(&Instance::BuildA)>::value << std::endl;
  // true
  std::cout << std::is_pointer<decltype(&Instance::BuildC)>::value << std::endl;
  void *buffer[2];
  std::cout << sizeof(buffer) << std::endl;
  
  return 0;
}
/**
 * C++ 函数指针 & 类成员函数指针
 * https://www.runoob.com/w3cnote/cpp-func-pointer.html
 * 指针异类：成员函数指针 & 数据成员指针
 * https://zhuanlan.zhihu.com/p/374358420
 */
