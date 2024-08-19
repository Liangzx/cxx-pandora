// https://blog.csdn.net/baidu_41388533/article/details/109692968
// https://blog.csdn.net/baidu_41388533/article/details/109694962

#include <boost/type_index.hpp>
#include <cstdio>
#include <cstdlib>
#include <iostream>

namespace lzx {
class A {
 public:
  A(int i)  // 构造函数
  {
    printf("A::A()函数执行了,this=%p\n", this);
  }

  A() = default;

  double myfunc()  // 普通成员函数
  {
    printf("A::myfunc()函数执行了,this=%p\n", this);
    return 12.1;
  }

 private:
  ~A() {}
};

using boost::typeindex::type_id_with_cvr;

void func1() {
  using YT = decltype(std::declval<lzx::A>());
  // 利用boost输出类型名比typeid(...).name()用法输出类型名更准确。

  std::cout << "YT=" << type_id_with_cvr<YT>().pretty_name()
            << std::endl;  // 显示YT的类型
}

// void func2() {
//   A myaobj(1); // 必须为构造函数提供参数
//   using boost::typeindex::type_id_with_cvr;
//   std::cout << "myaobj.myfunc()的返回类型="
//             << type_id_with_cvr<decltype(myaobj.myfunc())>().pretty_name()
//             << std::endl;

//   // 使用decltype获取类型不调用函数，也就是myfunc函数不执行。
//   //
//   缺点是这种方法需要创建A类对象，有没有什么方法不需要创建对象也能获取成员函数返回值类型？
//   using boost::typeindex::type_id_with_cvr;
//   std::cout
//       << "A::myfunc()的返回类型="
//       <<
//       type_id_with_cvr<decltype(std::declval<A>().myfunc())>().pretty_name()
//       << std::endl;
// }

void func3() {
  A &&ayinobj();  // 看起来是一个函数声明的语法
  // ayinobj();     // 看起来是调用ayinobj这个函数
  // 上面代码编译没有错误（即语法没问题），链接出错。
  //  ayinobj函数只是声明，没有定义，类似于std::declval函数。
  // A&&
  // ayinobj();该函数返回的类型是A&&，可以看成是返回了一个A&&类型的对象，这种对象就可以看成是类A对象，实现类成员函数调用
  // 结合decltype，编译链接都不会报错
  A &&ayinobj();
  decltype(ayinobj().myfunc()) mydblvar;  // 定义了一个double类型的变量mydblvar;
  // decltype(ayinobj().myfunc())
  // ：不会调用ayinobj函数，也不会调用myfunc函数，所以不会报错。
  // 同理，可理解decltype(std::declval<A>().myfunc())含义
}

// //  自己实现类似于declval的函数
// template <typename T> T mydeclval() noexcept;

// void func4() {
//   using boost::typeindex::type_id_with_cvr;
//   std::cout << "mydeclval<A>()的返回类型="
//             << type_id_with_cvr<decltype(mydeclval<A>())>().pretty_name()
//             << std::endl;
//   // std::cout
//   //     << "mydeclval<A>().myfunc()的返回类型="
//   //     <<
//   type_id_with_cvr<decltype(mydeclval<A>().myfunc())>().pretty_name()
//   //     << std::endl;
// }

// 返回左值引用还是返回右值引用
// template <typename T> T &mydeclval() noexcept;
template <typename T>
T &&mydeclval() noexcept;

void func5() {
  using boost::typeindex::type_id_with_cvr;

  std::cout << "decltype(mydeclval<A>)())的返回类型="
            << type_id_with_cvr<decltype(mydeclval<A>())>().pretty_name()
            << std::endl;
  std::cout << "decltype(mydeclval<A&>)())的返回类型="
            << type_id_with_cvr<decltype(mydeclval<A &>())>().pretty_name()
            << std::endl;
  std::cout << "decltype(mydeclval<A&&>)())的返回类型="
            << type_id_with_cvr<decltype(mydeclval<A &&>())>().pretty_name()
            << std::endl;

  // 输出（用到了引用折叠）
  // decltype(mydeclval<A>)())的返回类型 = class A &
  // decltype(mydeclval<A&>)())的返回类型 = class A &
  // decltype(mydeclval<A&&>)())的返回类型 = class A &
  // 通过上面两种方式对比可知：设定返回为右值引用，可以获得更多的类型，
  // 而设定返回左值应用不论传入什么类型的参数，返回全部的都是左值引用。
}

// 调用引用限定符修饰的成员函数范例
class ALR {
 public:
  void onAnyValue() {
    std::cout << "ALR::onAnyValue()函数执行了！" << std::endl;
  }

  void onLvalue() &  // 只能被类ALR的左值对象调用
  {
    std::cout << "ALR::onLvalue()函数执行了！" << std::endl;
  }

  void onRvalue() &&  // 只能被类ALR的右值对象调用
  {
    std::cout << "ALR::onRvalue()函数执行了！" << std::endl;
  }
};

void func6() {
  ALR alr;  // 左值对象alr
  alr.onLvalue();
  // alr.onRvalue(); //编译错误，因为onRvalue只能被类A的右值对象调用
  ALR().onRvalue();  // 临时对象是右值对象
  // ALR().onLvalue();  //编译错误，因为onLvalue只能被类A的左值对象调用
}

/*
void func7() {
  decltype(mydeclval<ALR>().onAnyValue());
  decltype(mydeclval<ALR &>()
               .onLvalue());  // 返回的 类型是class ALR
                              //
&，代表返回的是左值对象，左值对象调用onLvalue没问题 decltype(mydeclval<ALR &&>()
               .onRvalue());  // 返回的 类型是class ALR
                              //
&&，代表返回的是右值对象，右值对象调用onRvalue没问题
  // decltype(mydeclval<ALR&>().onRvalue());//返回的 类型是class ALR
  // &，代表返回的是左值对象，左值对象调用onRvalue是错误的
  // decltype(mydeclval<ALR&&>().onLvalue()); //返回的 类型是class ALR
  // &&，代表返回的是右值对象，右值对象调用onLvalue是错误的
}
*/

// 推导函数返回值范例
int myfunc(
    int a,
    int b)  // 函数类型一般是由函数的返回值和参数类型决定，与函数名没有关系，所以myfunc代表的函数类型是:
            // int(int,int);
{
  return a + b;
}

template <typename T_F, typename... U_Args>
decltype(std::declval<T_F>()(std::declval<U_Args>()...)) TestFnRtnImpl(
    T_F func, U_Args... args) {
  auto rtnvalue = func(args...);
  return rtnvalue;
}

void func8() {
  // T_F：是int (*)(int,int)类型，也就是函数指针类型
  // decltype(std::declval<T_F>()
  // (std::declval<U_Args>()...))：是int类型，也就是myfunc函数的返回类型
  // decltype(std::declval<T_F>() )：是 int (* &&
  // )(int,int),函数指针的右值引用类型，其实就简单理解成函数指针类型
}

}  // namespace lzx

// https://en.cppreference.com/w/cpp/utility/declval
struct Default {
  int foo() const { return 1; }
};

struct NonDefault {
  NonDefault() = delete;
  int foo() const { return 1; }
};

void func8() {
  decltype(Default().foo()) n1 = 1;  // type of n1 is int
  /// error: no default constructor
  // decltype(NonDefault().foo()) n12 = n1;
  decltype(std::declval<NonDefault>().foo()) n2 = n1;  // type of n2 is int
  std::cout << "n1 = " << n1 << '\n' << "n2 = " << n2 << '\n';
}

int main() {
  // lzx::func1();
  // lzx::func2();
  // lzx::func3();
  // lzx::func4();
  // lzx::func5();
  // lzx::func6();
  // lzx::func7();

  return 0;
}
