// https://en.cppreference.com/w/cpp/language/constraints
// https://developer.aliyun.com/article/1467317
// https://zhuanlan.zhihu.com/p/107610017
// requires表达式 ---C++ 20 模板
// https://zhuanlan.zhihu.com/p/516734341
// 一文读懂C++20新特性之概念、约束（concept, constraint）
// https://blog.csdn.net/Jxianxu/article/details/127400217

#include <concepts>

template<typename T>
concept Integral = std::is_integral<T>::value;

// 至少 4 种方式
Integral auto Add1(Integral auto a, Integral auto b)
{
  return a + b;
}

// 可以用新的格式：Integral auto c = Add(10, 3)来调用，
// 当然也可以用以前的调用方法：int c = Add(10, 3)
template<typename T>
  requires Integral<T>
T Add2(T a, T b)
{
  return a + b;
}

// 这种用法不必对函数声明中的每个T都写成长长的Integral auto，
// 当函数参数较多时明显比第一种要好，所以Bjarne Stroustrup也不见得是对的，
// 毕竟人都有偏见。

template<typename T>
T Add3(T a, T b)
  requires Integral<T>
{
  return a + b;
}

template<Integral T>
T Add4(T a, T b)
{
  return a + b;
}

int main()
{
  auto c = Add(10.3, 5.2);
  // 编译器给出类似这样的错误：
  // error: use of function 'T Add(T, T) [with T = double]' with unsatisfied
  // constraints
}
