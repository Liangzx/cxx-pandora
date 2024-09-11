#include <iostream>
#include <tuple>

// https://stackoverflow.com/questions/26902633/how-to-iterate-over-a-stdtuple-in-c-11/26902803#26902803

template<class F, class... Ts, std::size_t... Is>
void for_each_in_tuple(const std::tuple<Ts...>& tuple,
                       F func,
                       std::index_sequence<Is...>)
{
  using expander = int[];
  (void)expander{0, ((void)func(std::get<Is>(tuple)), 0)...};
}

template<class F, class... Ts>
void for_each_in_tuple(std::tuple<Ts...>& tuple, F func)
{
  for_each_in_tuple(tuple, func, std::make_index_sequence<sizeof...(Ts)>());
}

//---------------------
template<class... Fs>
void do_in_order(Fs&&... fs)
{
  int unused[] = {0, ((void)std::forward<Fs>(fs)(), 0)...};
  (void)unused; // blocks warnings
}

template<class T>
constexpr std::make_index_sequence<std::tuple_size<T>::value> get_indexes(
  T const&)
{
  return {};
}

template<size_t... Is, class Tuple, class F>
void for_each(std::index_sequence<Is...>, Tuple&& tup, F&& f)
{
  using std::get;
  do_in_order([&] {
    f(get<Is>(std::forward<Tuple>(tup)));
  }...);
}

template<class Tuple, class F>
void for_each(Tuple&& tup, F&& f)
{
  auto indexes = get_indexes(tup);
  for_each(indexes, std::forward<Tuple>(tup), std::forward<F>(f));
}

int main()
{
  auto some = std::make_tuple("I am good", 255, 2.1);
  // for_each_in_tuple(some, [](const auto& x) {
  //   std::cout << x << std::endl;
  // });

  //
  for_each(some, [](const auto& x) {
    std::cout << x << std::endl;
  });

  return 0;
}
