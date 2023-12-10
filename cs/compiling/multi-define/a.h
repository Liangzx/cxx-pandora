// test.h
#ifndef TEST_H
#define TEST_H
int a;                // 外部链接，不能在头文件中定义（是声明也是定义）
extern int b = 10;    // 外部链接，不能在头文件中定义（是定义）   extern int bb;   //声明，内部链接，可以定在头文件中（是声明）
const int c = 2;      // 内部链接，可以定在头文件中但应该避免
static int d = 3;     // 内部链接，可以定在头文件中但应该避免
static void func() {} // 同上
void func2() {}       // 同a
void func3();         // 可以，仅仅是声明。并不会导致符号名被写入目标文件。
class A               // 可以，类定义，内部链接
{
public:
    static int e; // 可以，声明，内部链接
    int f;        // 同上
    void func4(); // 同上
};
int A::e = 10;   // 不可以，外部链接，符号名会写入目标文件
void A ::func4() // 不可以，外部链接
{
    //......
}
#endif
