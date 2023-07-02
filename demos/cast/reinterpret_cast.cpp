#include <iostream>
#include <vector>
using namespace std;

class Other {};

class Base  // 父类
{
 public:
  virtual void f() { cout << "this is base class !" << endl; }
};

class Child : public Base {  // 子类
 public:
  int a = 10;

  string b = string("asd");

  void f() { cout << "this is child class !" << endl; }

  void CFun() {
    cout << "the first string is " << b[1] << endl;
    cout << "the value of  a in " << a << endl;
  }
};

int main() {
  std::vector<int> a;
  a.at(1);
  // eg 1 可以进行不相关类之间的转换，例如 Other与Base
  Other* o = new Other();
  Base* b = reinterpret_cast<Base*>(o);

  // eg 2 指针和整数进行转换
  int i_val = 100;
  int* i_ptr = &i_val;
  int64_t reinterpret_val = reinterpret_cast<int64_t>(i_ptr);  // 将整数指针转换为整数
  cout << reinterpret_val << endl;
  // 将整数转换为整数指针
  int* i_ptr1 = reinterpret_cast<int*>(reinterpret_val);
  cout << *i_ptr1 << endl;

  Base* b1 = new Base();
  int64_t reinterpret_val1 = reinterpret_cast<int64_t>(b1);  // 将Base指针转换为整数
  cout << reinterpret_val1 << endl;
  // 将对应整数转换为Base指针
  Base* b2 = reinterpret_cast<Base*>(reinterpret_val1);
  b2->f();  // 输出this is base class !
}
