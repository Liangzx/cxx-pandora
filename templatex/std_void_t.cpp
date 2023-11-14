#include <type_traits> // for true_type, false_type, and void_t
#include <iostream>

struct NoInnerType {
  int m_;
};

struct HaveInnerType {
  using type = int;
  void Func() {}
};

/*******************************判断类中是否存在某个类型别名 *************/

// 泛化版本
template <typename T, typename U = std::void_t<>>
struct HasMemType : std::false_type {};

// 特化版本
template <typename T>
struct HasMemType<T, std::void_t<typename T::type>> : std::true_type {};

// // 带参数的宏定义,注意 反斜杠 表示下一行接着本行来，是本行的一部分
// #define _HAS_TYPE_MEM_(parMTpNm) \
//   template <typename T, typename U = std::void_t<>> \
//   struct HTM_##parMTpNm : std::false_type {}; \
//   template <typename T> \
//   struct HTM_##parMTpNm<T, std::void_t<typename T::parMTpNm>> \
//       : std::true_type {};

// _HAS_TYPE_MEM_(type);
// _HAS_TYPE_MEM_(sizetype);

// /**
//  * cout <<  HTM_type<NoInnerType>::value << endl;
// cout << HTM_type<HaveInnerType>::value << endl;

// cout << HTM_sizetype<NoInnerType>::value << endl;
// cout << HTM_sizetype<HaveInnerType>::value << endl;

/****************************** 判断类中是否存在某个成员变量 *****************/
// 泛化版本
template <typename T, typename U = std::void_t<>>
struct HasMemVal : std::false_type {};
// 特化版本
template <typename T>
struct HasMemVal<T, std::void_t<decltype(&T::m_)>> : std::true_type {};

/****************************** 判断类中是否存在某个成员函数 *****************/

// 泛化版本
template <typename T, typename U = std::void_t<>>
struct HasMemFunc : std::false_type {};
// 特化版本
template <typename T>
struct HasMemFunc<T, std::void_t<decltype(std::declval<T>().Func())>>
    : std::true_type {};

/***************************************************************************/

int main() {
  std::cout << HasMemFunc<NoInnerType>::value << std::endl;
  std::cout << HasMemVal<NoInnerType>::value << std::endl;

  return 0;
}

/**
 * https://blog.csdn.net/baidu_41388533/article/details/109700163
 * http://www.tmplbook.com/code/traits/hasmember.hpp.html
 */