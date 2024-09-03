// 以下内容是头文件 tiny_size.h
#ifndef BOOST_PP_IS_ITERATING // 或者#if !BOOST_PP_IS_ITERATING
// 主体部分
#ifndef TINY_SIZE_H
#define TINY_SIZE_H

#include <boost/mpl/int.hpp>
#include <boost/preprocessor.hpp>

#ifndef TINY_MAX_SIZE
#define TINY_MAX_SIZE 5
#endif

struct none
{
};
// 定义类模板
template<BOOST_PP_ENUM_PARAMS(TINY_MAX_SIZE, typename T)>
struct tiny_size: mpl::int_<TINY_MAX_SIZE>
{
};

// 自迭代指令，注意指定的头文件是自己的头文件名
#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, TINY_MAX_SIZE - 1, "tiny_size.h"))
#include BOOST_PP_ITERATE()

#endif // TINY_SIZE_H

#else
// 迭代部分
#define n BOOST_PP_ITERATION()
#define Tiny_print(z, x, data) data

// 模板特化定义，注意没有使用#define，不需要’\’，而迭代序号n通过BOOST_PP_ITERATION得到
template<BOOST_PP_ENUM_PARAMS(n, typename T)>
struct tiny_size<BOOST_PP_ENUM_PARAMS(n, T) BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM(
  BOOST_PP_SUB(TINY_MAX_SIZE, n), Tiny_print, none)>: mpl::int_<n>
{
};

#undef Tiny_print
#undef n

#endif
