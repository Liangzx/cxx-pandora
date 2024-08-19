// https://blog.csdn.net/y1196645376/article/details/51441503

void func() {
  int i = 0, &r = i;  // 定义一个整数i,并且定义r为i的应用.
  auto a = r;         // 这里的a为为一个整数,其值跟此时的i一样.
  decltype(r) c = i;  // c->int&
}

void func1() {
  int i = 0;
  const int ci = i, &cr = ci;  // ci 为整数常量,cr 为整数常量引用
  auto a = ci;                 // a 为一个整数, 顶层const被忽略
  auto b = cr;                 // b 为一个整数，顶层const被忽略
  auto c = &ci;                // c 为一个整数指针.
  /// d 为一个指向整数常量的指针(对常量对象取地址时那么const会变成底层const)
  auto d = &cr;  // const int *d
}

void func2() {
  int i = 0;
  const int ci = i, &cr = ci;  // ci 为整数常量,cr 为整数常量引用
  //  如果你希望推断出auto类型是一个顶层的const，需要明确指出
  const auto f = ci;  // const int f
  // 还可以将引用的类型设为auto,此时原来的初始化规则仍然适用(用于引用声明的const都是底层const)
  auto &g = ci;  // g是一个整数常量引用，绑定到ci。
  //   auto &h = 42;        // 错误：非常量引用的初始值必须为左值。
  const auto &j = 42;  // 正确:常量引用可以绑定到字面值。
}

int main() { return 0; }

/**
 * 1. 顶层 const (Top-level const)
 * 顶层 const 是作用于整个对象的 const，它表示整个对象是只读的。
 * 2. 底层 const (Bottom-level const)
 * 底层 const 是作用于对象内部的 const，它表示对象的某个部分是只读的
 * 3. 底层 const 和顶层 const 的区别
 *  顶层 const：表示整个对象是只读的，不能被修改。
 * (常量指针，指针常量)
    底层 const：
        当 const
 放在指针后面时，表示指针所指向的对象是只读的，不能通过该指针修改对象。 当 const
 放在指针前面时，表示指针自身是只读的，不能改变指针指向的对象。
 */
#include <iostream>
void func_const() {
  int i = 10;

  // 顶层 const
  int const i_const = 20;  // i_const 是只读的
  std::cout << "i_const: " << i_const << std::endl;

  // 底层 const
  int const *p = &i;  // p 指向的整数是只读的
  //   *p = 30;            // 编译错误：不能通过 p 修改 i
  p = &i_const;  // 正确：可以改变 p 指向的对象

  int *const q = &i;  // q 自身是只读的
  *q = 40;            // 正确：可以通过 q 修改 i
  //   q = &i_const;       // 编译错误：不能改变 q 指向的对象

  std::cout << "i: " << i << std::endl;
}

// 如果给这个变量加上一个或多层括号，那么编译器会把这个变量当作一个表达式看待，
// 变量是一个可以作为左值的特殊表达式，所以这样的decltype就会返回引用类型
void funca() {
  int i = 42;

  decltype(i) a;        // int  类型
  decltype((i)) b = i;  // int& 类型
}

// = 赋值运算符返回的是左值的引用。换句话意思就是说 decltype(i = b)  返回类型为
// i 类型的引用
void funcb() {
  int i = 42;

  decltype(i = 41) x = i;

  auto y = i;

  auto &z = i;

  printf("i x y z 此时为： %d %d %d %d\n", i, x, y, z);

  i--;

  printf("i x y z 此时为： %d %d %d %d\n", i, x, y, z);

  x--;

  printf("i x y z 此时为： %d %d %d %d\n", i, x, y, z);

  y--;

  printf("i x y z 此时为： %d %d %d %d\n", i, x, y, z);

  z--;

  printf("i x y z 此时为： %d %d %d %d\n", i, x, y, z);
}

// 在 C++ 中，左值（lvalue）是指可以出现在赋值表达式左边的表达式。
// 通常，左值指的是具有持久存储位置的对象，如变量。然而，C++ 中也有
// 一些特殊的表达式可以作为左值出现，即使它们本身并不是变量。
struct S {
  int x;
};

int &getRef() {
  static int i = 0;
  return i;
}

void funcc() {
  int i = 10;
  int arr[5] = {0};
  S s{.x = 0};

  // 数组下标表达式作为左值
  arr[0] = 10;

  // 成员访问表达式作为左值
  s.x = 20;

  // 引用作为左值
  int &ref = i;
  ref = 30;

  // 指针解引用作为左值
  int *ptr = &i;
  *ptr = 40;

  // 函数返回值作为左值
  getRef() = 50;

  // std::ref 将右值转换为左值
  std::ref(i) = 60;

  // std::move 的结果作为左值（C++17 及以后版本）
  std::move(i) = 70;

  std::cout << "i: " << i << std::endl;
  std::cout << "arr[0]: " << arr[0] << std::endl;
  std::cout << "s.x: " << s.x << std::endl;
}

// [decltype,std::declval,std::result_of, std::invoke_result](https://blog.csdn.net/zhaofuguang/article/details/134876749)
