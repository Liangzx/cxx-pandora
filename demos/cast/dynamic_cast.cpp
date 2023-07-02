#include <iostream>
using namespace std;

class Base  // 父类
{};

class Child : public Base {  // 子类
};

/**
 *  dynamic_cast:
 *  1. 主要是用于有继承关系的，或者说实现存在多态的父类和子类之间的转换。
 *     如果两个类是继承关系，但是没有多态的存在（也就是没有虚函数），
 *     那么使用dynamic_cast进行转换，编译器会报错的
 */

void dynamic_cast_eg1() {
//  Base* b_ptr = new Child();
//
//  Child* c_ptr = dynamic_cast<Child*>(
//      b_ptr);  //这里会报错，在vs2019中，提示：运行时 dynamic_cast
//               // 的操作数必须包含多态类类型
}

int main() {}
