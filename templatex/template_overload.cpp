#include <iostream>
#include <list>
#include <vector>

template<typename T>
bool operator== (std::vector<T> const& a, std::vector<T> const& b)
{
  return false;
}

template<typename T>
bool operator== (std::list<T> const& a, std::list<T> const& b)
{
  return false;
}

// /// 冲突
// template<typename U>
// bool operator== (std::vector<U> const& a, std::vector<U> const& b)
// {
//   return true;
// }

template<typename T>
void func(T a)
{
  // 模板函数体
}

template<typename T>
void func(T a, T b)
{ // 错误：试图通过改变模板参数来重载
  // 模板函数体
}

int main()
{

  return 0;
}
