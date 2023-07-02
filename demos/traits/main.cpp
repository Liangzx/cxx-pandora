#include <iostream>

#include "tr_conjunction_v.h"
#include "tr_enable_if.h"
#include "tr_is_valid.h"

// std::enable_if
// constexpr auto isDefaultConstructible= [](auto &&...args)
//{
//  auto ff = [](auto x)-> decltype((void)decltype(valueT(x))()){};
//  return decltype(isValidImpl<
//                  decltype(ff),
//                  decltype(args) &&...>(nullptr)){};
//};

constexpr auto isDefaultConstructible =
    isValid([](auto x) -> decltype((void)decltype(valueT(x))()) {});

template <typename T, typename = std::void_t<>>
struct HasFoo : std::false_type {};

template <typename T>
struct HasFoo<T, std::void_t<decltype(std::declval<T>().toString())>>
    : std::true_type {};

namespace detail {
template <typename Void, typename Obj, typename Func>
struct HasBegin : std::false_type {};

template <typename Obj, typename Func>
struct HasBegin<std::void_t<decltype(static_cast<Func Obj::*>(&Obj::begin))>,
                Obj, Func> : std::true_type {};
}  // namespace detail

template <typename Obj, typename Func>
using HasBegin = detail::HasBegin<void, Obj, Func>;

struct A {
  int begin(int a){return a;}
};

using HelloFunc = int(int);

int main() {
  std::cout << HasBegin<A, HelloFunc>::value << std::endl;
  return 0;
}