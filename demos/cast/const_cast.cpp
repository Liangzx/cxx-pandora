#include <stdio.h>

#include <iostream>

using namespace std;

void fun(int* a) { cout << *a << endl; }

/**
 * const_cast：
 *  主要是用来去除复合类型中const属性
 */

class myClass {
 public:
  int a = 10;
};

void eg1() {
  const myClass* c = new myClass();
  cout << c->a << endl;
  // c->a = 100; // failed,const常量不能修改
  myClass* cc = const_cast<myClass*>(c);
  cc->a = 100;  // success
  cout << c->a << " " << cc->a << endl;
}

/**
 * 在我们自定义的类中，去除const属性之后，可以修改原来的值，
 * 一般是通过转换为指针或者引用进行修改如 eg1
 * 如果不是自定义类，比如整数，字符串指针等，就会有意想不到的报错或者奇怪现象
 */
void eg2() {
  /**
   * data 的地址与p指向的地址相同但是修改 p 之后, 同一个地址上的内容却不相同。
   * 这是由于const机制， const 的机制，就是在编译期间，用一个常量代替了 data。
   * 这种方式叫做常量折叠。常量折叠与编译器的工作原理有关，是编译器的一种编译优化。
   * 在编译器进行语法分析的时候，将常量表达式计算求值，并用求得的值来替换表达式，放入常量表。
   * 所以在上面的例子中，编译器在优化的过程中，会把碰到的data（为const常量）全部以内容100替换掉，
   * 跟宏的替换有点类似。常量折叠只对原生类型起作用，对我们自定义的类型，是不会起作用的。
   */
  const int data = 100;
  int* pp = (int*)&data;
  *pp = 300;
  std::cout << "---------------------------------" << std::endl;
  cout << "data   = " << data << "\t地址 : " << &data << std::endl;
  cout << "  pp   = " << *pp << "\t地址 : " << pp << std::endl;
  std::cout << "---------------------------------" << std::endl;
  int* p = const_cast<int*>(&data);
  cout << "data   = " << data << "\t地址 : " << &data << std::endl;
  cout << "  p    = " << *p << "\t地址 : " << p << std::endl;
  *p = 200;

  cout << "data   = " << data << "\t地址 : " << &data << std::endl;
  cout << "  p    = " << *p << "\t地址 : " << p << std::endl;
}

int main() {
  //  int a = 10;
  //  const int* a_ptr = &a;
  //  // fun(a_ptr); // failed, 参数类型不对，需要非const 类型的参数
  //  // success,转换为非const 类型的参数，可以进行函数调用
  //  fun(const_cast<int*>(a_ptr));
  eg2();

  return 0;
}
