// [C++11改进我们的程序之简化我们的程序（七）]
// https://www.cnblogs.com/qicosmos/p/3318070.html

#include <complex>
#include <iostream>
#include <map>
#include <string>
#include <tuple>

using namespace std;

void func1()
{
  auto tp = std::make_tuple(42, 3.14, "Hello, World!");
  int x;
  double y;
  string a;
  std::tie(x, y, a) = tp;
  std::tie(std::ignore, y, std::ignore) = tp; // 只解第三个值了
  std::cout << y << std::endl;
}

void func2()
{
  // https://en.cppreference.com/w/cpp/utility/tuple/forward_as_tuple

  std::map<int, std::string> m;

  m.emplace(std::piecewise_construct,
            std::forward_as_tuple(10),
            std::forward_as_tuple(20, 'a'));
  std::cout << "m[10] = " << m[10] << '\n';
  std::cout << m.size() << std::endl;

  // The following is an error: it produces a
  // std::tuple<int&&, char&&> holding two dangling references.
  //
  // auto t = std::forward_as_tuple(20, 'a');
  // m.emplace(std::piecewise_construct, std::forward_as_tuple(10), t);
}

void func3()
{

  // C++11 中 std::piecewise_construct 的使用
  // https://juejin.cn/post/7029372430397210632
  // @note 无法区分哪个参数用来构造 key 哪些用来构造 value
  // 有可能是 std::string s("hello", 1), std::complex<double> cpx(2)
  // 也有可能是 std::string s("hello"), std::complex<double> cpx(1, 2)
  std::map<std::string, std::complex<double>> scp;
  //   scp.emplace("hello", 1, 2);

  // 此常量值作为构造 pair
  // 对象的第一个参数传递，以选择构造函数形式，通过将两个元组对象的元素转发给它们各自的构造函数来构造其成员。
  // 该函数会帮你构造一个 tuple 并转发给 pair 构造，并存储在 first 字段
  // 该函数会帮你构造一个 tuple 并转发给 pair 构造，存储在 second 字段
  scp.emplace(std::piecewise_construct,
              std::forward_as_tuple("hello"),
              std::forward_as_tuple(1, 2));

  std::cout << scp["hello"] << std::endl;
}

// 遍历tuple中的每个元素
template<class Tuple, std::size_t N>
struct TuplePrinter
{
  static void print(const Tuple& t)
  {
    TuplePrinter<Tuple, N - 1>::print(t);
    std::cout << ", " << std::get<N - 1>(t);
  }
};

template<class Tuple>
struct TuplePrinter<Tuple, 1>
{
  static void print(const Tuple& t)
  {
    std::cout << std::get<0>(t);
  }
};

template<class... Args>
void PrintTuple(const std::tuple<Args...>& t)
{
  std::cout << "(";
  TuplePrinter<decltype(t), sizeof...(Args)>::print(t);
  std::cout << ")\n";
}

void func4()
{
  auto tp = std::make_tuple(42, 3.14, "Hello, World!");
  PrintTuple(tp);
}

// 根据tuple元素值获取其对应的索引位置
namespace detail
{
template<int I, typename T, typename... Args>
struct find_index
{
  static int call(std::tuple<Args...> const& t, T&& val)
  {
    return (std::get<I - 1>(t) == val)
             ? I - 1
             : find_index<I - 1, T, Args...>::call(t, std::forward<T>(val));
  }
};

template<typename T, typename... Args>
struct find_index<0, T, Args...>
{
  static int call(std::tuple<Args...> const& t, T&& val)
  {
    return (std::get<0>(t) == val) ? 0 : -1;
  }
};
} // namespace detail

template<typename T, typename... Args>
int find_index(std::tuple<Args...> const& t, T&& val)
{
  return detail::find_index<sizeof...(Args), T, Args...>::call(
    t,
    std::forward<T>(val));
}

void func5()
{
  auto tp = std::make_tuple(42, 3.14, 77);
  std::cout << find_index(tp, 77) << std::endl;
}

// 展开tuple，并将tuple元素作为函数的参数。这样就可以根据需要对tuple元素进行处理了
#include <tuple>
#include <type_traits>
#include <utility>

template<size_t N>
struct Apply
{
  template<typename F, typename T, typename... A>
  static inline auto apply(F&& f, T&& t, A&&... a)
  {
    return Apply<N - 1>::apply(std::forward<F>(f),
                               std::forward<T>(t),
                               std::get<N - 1>(std::forward<T>(t)),
                               std::forward<A>(a)...);
  }
};

template<>
struct Apply<0>
{
  template<typename F, typename T, typename... A>
  static inline auto apply(F&& f, T&&, A&&... a)
  {
    return std::forward<F>(f)(std::forward<A>(a)...);
  }
};

template<typename F, typename T>
inline auto my_apply(F&& f, T&& t)
{
  return Apply<std::tuple_size<typename std::decay<T>::type>::value>::apply(
    std::forward<F>(f),
    std::forward<T>(t));
}

void one(int i, double d)
{
  std::cout << "function one(" << i << ", " << d << ");\n";
}

int two(int i)
{
  std::cout << "function two(" << i << ");\n";
  return i;
}

void func6()
{
  std::tuple<int, double> tup(23, 4.5);
  my_apply(one, tup);

  // int d = my_apply(two, std::make_tuple(2));
}

int main()
{
  //   func1();
  //   func2();
  //   func4();
  // func5();
  func6();

  return 0;
}
