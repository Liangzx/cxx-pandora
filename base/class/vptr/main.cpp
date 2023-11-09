#include <iostream>

class Base1 {
public:
  int base1_1;
  int base1_2;
};

class Base2 {
public:
  int base2_1;
  int base2_2;

  virtual void base2_fun1() {}
  virtual void base2_fun2() {}
};

// 多继承
class Derive1 : public Base1, public Base2 {
public:
  int derive1_1;
  int derive1_2;

  // 自身定义的虚函数
  virtual void derive1_fun1() {}
  virtual void derive1_fun2() {}
};

int main() { 
  Derive1 d;
  std::cout << "sizeof(d): " << sizeof(d) << std::endl;
  std::cout << "offsetof(Derive1, base1_1): " << offsetof(Derive1, base1_1) << std::endl;
  std::cout << "offsetof(Derive1, base2_1): " << offsetof(Derive1, base2_1) << std::endl;
  std::cout << "offsetof(Derive1, derive1_1): " << offsetof(Derive1, derive1_1) << std::endl;


  return 0; 
}