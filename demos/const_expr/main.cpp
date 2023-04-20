#include <iostream>


template<int N>
class C {};

constexpr int FivePlus(int x) { return 5 + x; }

void f(const int x) {
//    C<x> c1;          // Error: x is not compile-time evaluable.
    C<FivePlus(6)> c2;// OK
}

void g() {
    const int x = 5;
    C<x> c1;          // OK!!! 此处用x的「常量」语义
    *(int *) (&x) = 6;// Still OK! 只处用x的「只读」语义，去除const后可写
    C<x> c2;          // Still OK! c2是C<5>类型（不是C<6>！）
    C<FivePlus(x)> c3;// Still OK! c3是C<10>类型（不是C<11>！）

    printf("%d\n", x);// 此处绝大多数（所有？）C++编译器会输出5！！
                      // （然而，如果用一个C编译器来编译类似代码，一定输出6）
    const int *p = &x;
    printf("%d\n", *p);// 此处，大多数C++编译器输出6
}


void const_f() {
    const int a = 10;
    int *p = (int *) (&a);
    *p = 30;
    /**
     * c(gcc) 输出30, c++(g++) 输出10
     * 原因：
     * 1. const 在 c 中的语义为只读，通过指针去除了 const 后变为可写
     * 2. const 在 c++ 中还有常量的语义
     */
    printf("%d\n", a);
}


int main(int argc, char **argv) {
    const_f();

    return 0;
}
/**
 * ref:
 *  https://www.zhihu.com/question/35614219
 *  https://zhuanlan.zhihu.com/p/20206577
 */
