#include <iostream>
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

enum class Fruit { Apple, Orange, Banana, Watermelon };
/**
 * static_cast
 * 静态转换，没有安全检查可以实现：
 * 1. 继承父类和子类之间的上行转换和下行转换，不保证安全，例如下述代码的eg2
 * 2. 能够进行void * 到其他指针的任意转换，例如下述代码的eg3
 * 3. 能够将 int float double以及枚举类型的转换，例如下述代码的eg4
 * 4. 实现转换到右值引用，例如下述代码的eg5
 */
int main() {
  // eg1 不能进行不相关类之间的转换，例如 Other与Base
  Other* o = new Other();
  Base* b = static_cast<Base*>(o);  // faild,这里会报错

  // eg2
  // 能够进行上行转换（upcast）和下行转换(downcast)，不保证安全，需要程序员自己去保证安全
  Base* b1 = new Base();
  Child* c1 = new Child();
  Child* c11 = static_cast<Child*>(b1);  // 下行转换，不安全，需要程序员自己保证
  // c11->CFun(); // failed
  // 这里调用CFun，由于基类里面没有对应a变量和b变量，所以肯定会报错
  Base* b11 = static_cast<Base*>(c1);  // 上行转，安全

  // eg 3 能够进行void * 到其他指针的任意转换
  void* p = nullptr;
  Base* b2 = static_cast<Base*>(p);   // 这里是将void * 转换为 Base*
  void* p1 = static_cast<void*>(b1);  // 这里是将Base* 转换为 void *

  // eg4 能够将 int float double以及枚举类型的转换
  Fruit f = Fruit::Apple;
  int i_value = static_cast<int>(f);
  Fruit ff = static_cast<Fruit>(3);  // 将整数3转换为Fruit::Watermelon
  int i_val = 100;
  double d_val = 1.000;
  float f_val = 2.000;
  int i_value1 = static_cast<int>(d_val);      // double 向 int 进行转换
  int i_value2 = static_cast<int>(f_val);      // float 向 int 进行转换
  float f_val1 = static_cast<float>(i_val);    // int 向 double 进行转换
  double d_val1 = static_cast<double>(i_val);  // int 向 float 进行转换

  // eg5 实现转换到右值引用，将左值转换为右值，和std::move功能差不多
  int&& left_val = static_cast<int&&>(5);
}
