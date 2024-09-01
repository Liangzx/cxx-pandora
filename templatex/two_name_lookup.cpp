// 模板的两段名字查找
// https://conzxy.github.io/2020/06/25/Cpp/Name%20lookup/Template%20define/
// Template的名字查找（OL and ADL）
// 普通名字查找（Ordinary Name Lookup，简称 OL）
// 依赖查找（Argument-Dependent Lookup，简称 ADL）

#include <iostream>

#include <iostream>
using namespace std;

struct C {};

void f(C) { cout << "C" << endl; }

template <typename T> void f(T) { cout << "T" << endl; }

template <typename T> class A {
public:
  void f() { cout << "A::f()" << endl; }
};

template <typename T> class B : public A<T> {
public:
  void g() {
    f(C());    // gloabl f():f non-template declaration
    this->f(); // A::f()
    A<T>::f(); // A::f()
  }
};

//--------
template <typename T> void adl(T) { cout << "T"; }

struct S {};

// void adl(S) { cout << "S"; }

template <typename T> void call_adl(T t) {
  adl(S());
  adl(t);
}

void adl(S) { cout << "S"; }

int main() {
  B<int> b;
  b.g();
  // Why the result is “TS”?
  /**
   * adl(S())是non-dependent name，只是normal lookup，non-template版本在被调用点以下，
   * 那么在实例化阶段才会可见，此时只有template版本可以绑定，所以输出”T”。
   * adl(t)是dependent name，按ADL查找，那么有两个函数可以匹配，但非模板优于模板，所以输出”S”。
   * 如果将非模板版本调至调用点之前，则输出”SS”
   */
  call_adl(S());
}

/**
 * 模板的名字查找分两个阶段：定义阶段和实例化阶段
 * 1.from the template definition: for non-dependent names.
 * 2.during instantiation: from the definition but also, via ADL only, from the instantiation context.
 * 定义阶段:
 * For a non-dependent name used in a template definition, unqualified name
 *  lookup takes place when the template definition is examined.
 * The binding to the declarations made at that point is not affected by declarations
 *  visible at the point of instantiation. in other words, adding a new function declaration
 *  after template definition does not make it visible, except via ADL
 * --所以non-dependent name调用点之后的函数声明是不可见的。
 */
