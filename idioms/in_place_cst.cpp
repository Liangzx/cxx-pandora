#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>
#include <optional>

using namespace std;

// // 定义标签类型（类似标准库的in_place_t）
// struct in_place_t {
//   explicit in_place_t() = default;
// };
// inline constexpr in_place_t in_place{};

// // 类型特定的标签（类似in_place_type_t）
// template <typename T> struct in_place_type_t {
//   explicit in_place_type_t() = default;
// };
// template <typename T> inline constexpr in_place_type_t<T> in_place_type{};

// // 索引特定的标签（类似in_place_index_t）
// template <size_t I> struct in_place_index_t {
//   explicit in_place_index_t() = default;
// };
// template <size_t I> inline constexpr in_place_index_t<I> in_place_index{};

template <typename T> class MyOptional {
  alignas(T) unsigned char storage[sizeof(T)];
  bool engaged = false;

public:
  // 常规构造函数
  explicit MyOptional() = default;

  // in_place构造（无参数）
  explicit MyOptional(in_place_t) : engaged(true) { new (storage) T(); }

  // in_place构造（带参数）
  template <typename... Args>
  explicit MyOptional(in_place_t, Args &&... args) : engaged(true) {
    new (storage) T(std::forward<Args>(args)...);
  }

  // 类型特定的in_place构造
  template <typename U, typename... Args>
  explicit MyOptional(in_place_type_t<U>, Args &&... args) : engaged(true) {
    static_assert(std::is_same_v<T, U>, "Type mismatch");
    new (storage) T(std::forward<Args>(args)...);
  }

  ~MyOptional() {
    if (engaged) {
      reinterpret_cast<T *>(storage)->~T();
    }
  }

  // 访问器
  T &operator*() & { return *reinterpret_cast<T *>(storage); }
  const T &operator*() const & { return *reinterpret_cast<const T *>(storage); }
};

struct Point {
  int x, y;
  Point(int a, int b) : x(a), y(b) {}
};

struct Test {
  Test(int, int) { std::cout << "普通构造\n"; }
  Test(std::initializer_list<int>, int) { std::cout << "列表构造\n"; }
};

int main() {
  // 常规构造
  MyOptional<int> empty;

  // in_place无参数构造
  MyOptional<int> opt1(std::in_place);

  // in_place带参数构造
  MyOptional<Point> opt2(std::in_place, 10, 20);

  // 类型特定的in_place构造
  MyOptional<Point> opt3(in_place_type<Point>, 30, 40);

  std::cout
      << (*opt2).x << "," << (*opt2).y
      << std::endl; // 输出: 10,20
                    //   std::cout << opt3->x << "," << opt3->y; // 输出: 30,40

  std::optional<Test> t1(std::in_place, 1, 2);      // 输出"普通构造"
  std::optional<Test> t2(std::in_place, {1, 2}, 3); // 输出"列表构造"
}
