// C++父类与子类间构造，析构，拷贝的调用关系
// https://blog.csdn.net/WChQGouge/article/details/99873343

#include <cstring>
#include <iostream>
#include <stdio.h>
using namespace std;

class Base {
public:
  char *str;

  Base(void) { cout << "父类无参构造" << endl; }
  Base(const char *str) {
    this->str = new char[strlen(str) + 1];
    cout << "父类有参构造" << str << endl;
  }
  Base(Base &that) { cout << "父类拷贝构造" << endl; }
  Base(Base &&that) { cout << "父类移动构造" << endl; }
};

class A : public Base {
public:
  A(void) { cout << "子类无参构造" << endl; }
  A(const char *str) : Base(str) { cout << "子类有参构造" << str << endl; }
  //   A(A &that) : Base(that) { cout << "子类拷贝构造" << endl; }
  A(A &that) { cout << "子类拷贝构造" << endl; }
  // A(A &&that) { cout << "子类移动构造" << endl; }
  A(A &&that):Base(std::move(that)) { cout << "子类移动构造" << endl; }
};

int main() {
  A a("x");
  cout << "------" << endl;
  A a1 =std::move(a);
  cout << "------" << a.str << endl;
}
