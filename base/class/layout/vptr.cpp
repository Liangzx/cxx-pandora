// demo vptr.cpp
#include <iostream>

class Base {
 public:
  virtual void funcA() { std::cout << "Base::funcA" << std::endl; }
  virtual void funcB() { std::cout << "Base::funcB" << std::endl; }
};

class A : public Base {
 public:
  virtual void funcA() { std::cout << "A::funcA" << std::endl; }
  void funcB() { std::cout << "A:funcB" << std::endl; }
};

int main() {
  std::cout << "Base " << sizeof(Base) << " A " << sizeof(A) << std::endl;

  return 0;
}

// win: cl person.cpp -d1 reportSingleClassLayouttest
// linux: g++ -fdump-class-hierarchy vptr.cpp (GCC 7.X 以下)
// g++ -fdump-lang-class vptr.cpp (8.0+)
