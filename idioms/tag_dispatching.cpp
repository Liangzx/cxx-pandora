// C++ 的 Tag Dispatching(标签派发) 惯用法
// https://blog.csdn.net/haokan123456789/article/details/139393256

#include <iostream>
#include <string>

// // 1. 使用 type traits 技术
// struct NumTag
// {
// };

// struct StrTag
// {
// };

// template<typename T>
// struct traits
// {
//   using tag = NumTag;
// };

// template<>
// struct traits<std::string>
// {
//   using tag = StrTag;
// };

// template<>
// struct traits<const char*>
// {
//   using tag = StrTag;
// };

// template<typename T>
// std::string ToString_impl(T t, NumTag)
// {
//   return std::to_string(t);
// }

// template<typename T>
// std::string ToString_impl(T t, StrTag)
// {
//   return t;
// }

// template<typename T>
// std::string ToString(T t)
// {
//   return ToString_impl(t, typename traits<T>::tag());
// }

// 2. 使用 Type_2_Type 技术
template<typename T>
struct TypeTag
{
};

template<typename T>
auto ToString_impl(T t, TypeTag<int>)
{
  return std::to_string(t);
}

template<typename T>
auto ToString_impl(T t, TypeTag<std::string>)
{
  return t;
}

template<typename T>
auto ToString_impl(T t, TypeTag<const char*>)
{
  return t;
}

template<typename T>
auto ToString(T t)
{
  return ToString_impl(t, TypeTag<T>());
}

int main()
{
  std::cout << ToString(42) << std::endl;
  std::cout << ToString("Emma") << std::endl;
}
