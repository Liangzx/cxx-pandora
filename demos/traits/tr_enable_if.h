//
// Created by dingjia on 2023/6/14.
//
/**
 * 原理：
 *  只有当第一个模板参数为true时，enable_if会包含一个type=T的公有成员，否则没有该公有成员
 *
 * 使用场景：
 *  1. 限制模板函数的参数类型
 *  2. 模板类型偏特化
 *
 * 参考:
 *  1. https://en.cppreference.com/w/cpp/types/enable_if
 *  2. https://www.cnblogs.com/3d-gis/articles/14392648.html
 *  3. https://blog.csdn.net/netyeaxi/article/details/83479646
 */
#ifndef DEMOS_TR_ENABLE_IF_H
/******************** 限制模板函数的参数类型 ********************/
// 1. the return type (bool) is only valid if T is an integral type
template <class T>
typename std::enable_if<std::is_integral<T>::value, bool>::type is_odd(T i) {
  return bool(i % 2);
}

// 2. the second template argument is only valid if T is an integral type
template <class T,
          class = typename std::enable_if<std::is_integral<T>::value>::type>
bool is_even(T i) {
  return !bool(i % 2);
}

/*********************** 模板类型偏特化 ************************/
#include <iostream>
#include <memory>
#include <type_traits>

template <typename T>
struct is_smart_pointer_helper : public std::false_type {};

template <typename T>
struct is_smart_pointer_helper<std::shared_ptr<T> > : public std::true_type {};

template <typename T>
struct is_smart_pointer_helper<std::unique_ptr<T> > : public std::true_type {};

template <typename T>
struct is_smart_pointer_helper<std::weak_ptr<T> > : public std::true_type {};

template <typename T>
struct is_smart_pointer
    : public is_smart_pointer_helper<typename std::remove_cv<T>::type> {};

template <typename T>
typename std::enable_if<is_smart_pointer<T>::value, void>::type
check_smart_pointer(const T& t) {
  std::cout << "is smart pointer" << std::endl;
}

template <typename T>
typename std::enable_if<!is_smart_pointer<T>::value, void>::type
check_smart_pointer(const T& t) {
  std::cout << "not smart pointer" << std::endl;
}

#include <iostream>
#include <string>

using namespace std;

class Box {
 public:
  string str() { return "yes"; }
};

class Bin {
 public:
  string str1() { return "no"; }
};

template <typename U>
struct class_str {
  template <typename T, string (T::*)() = &T::str>
  static constexpr bool check(T*) {
    return true;
  };

  static constexpr bool check(...) { return false; };

  static constexpr bool ret = check(static_cast<U*>(0));
};

// 不含有string str()方法的非std::string类
template <typename T,
          typename std::enable_if<std::is_class<T>::value &&
                                      !std::is_same<T, string>::value,
                                  T>::type* = nullptr,
          typename std::enable_if<!class_str<T>::ret, T>::type* = nullptr>
std::string str(T& t) {
  cout << "1.---------------------" << endl;
  return "null";
};

// std::string类
template <typename T,
          typename std::enable_if<std::is_class<T>::value &&
                                      std::is_same<T, string>::value,
                                  T>::type* = nullptr>
std::string str(T& t) {
  cout << "2.---------------------" << endl;
  return t;
};

// 含有string str()方法的非std::string类
template <typename T,
          typename std::enable_if<std::is_class<T>::value &&
                                      !std::is_same<T, string>::value,
                                  T>::type* = nullptr,
          typename std::enable_if<class_str<T>::ret, T>::type* = nullptr>
std::string str(T& t) {
  cout << "3.---------------------" << endl;
  return t.str();
};

// 数值型
template <typename T, typename std::enable_if<!std::is_class<T>::value &&
                                                  std::is_arithmetic<T>::value,
                                              T>::type* = nullptr>
std::string str(T& t) {
  cout << "4.---------------------" << endl;
  return std::to_string(t);
};

int main() {
  string s = "sddds";
  cout << str<string>(s) << endl;

  bool j = true;
  cout << str<bool>(j) << endl;

  int i = 1000;
  cout << str<int>(i) << endl;

  float f = 10.6f;
  cout << str<float>(f) << endl;

  Box b1;
  cout << str<Box>(b1) << endl;

  Bin b2;
  std::cout << "--------------" << std::endl;
  cout << str<Bin>(b2) << endl;

  return 1;
}

#define DEMOS_TR_ENABLE_IF_H

#endif  // DEMOS_TR_ENABLE_IF_H
