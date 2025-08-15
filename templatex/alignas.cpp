#include <iostream>
#include <memory>

#define SHOW_SIZEOF_AND_ALIGNOF(T)                                             \
  do {                                                                         \
    std::cout << "sizeof(" << #T << "):\t" << sizeof(T) << ",\t"               \
              << "alignof(" << #T << "):\t" << alignof(T) << std::endl;        \
  } while (0)

struct Foo {
  char c;
  int i1;
  int i2;
  long l;
};

struct alignas(32) Foo32 {
  explicit Foo32() { std::cout << this << std::endl; }
  char c;
  int i1;
  int i2;
  long l;
};

struct alignas(4) Foo4 {
  char c;
  int i1;
  int i2;
  long l;
};

// // alignment == pow(2, N)
// struct alignas(9) Foo9 {
//   char c;
//   int i1;
//   int i2;
//   long l;
// };

#define CHECK_ALIGN(ptr, alignment)                                            \
  do {                                                                         \
    if (reinterpret_cast<uintptr_t>(ptr) % (alignment) != 0) {                 \
      std::cerr << "Alignment error at: " << __LINE__ << std::endl;            \
      std::abort();                                                            \
    }                                                                          \
  } while (0)

int func1() {
  char c;
  int i;
  long l;
  float f;
  CHECK_ALIGN(&c, sizeof(c));
  CHECK_ALIGN(&i, sizeof(i));
  CHECK_ALIGN(&l, sizeof(l));
  CHECK_ALIGN(&f, sizeof(f));
  CHECK_ALIGN(&i, sizeof(l)); // 编译错误
  return 0;
}

int func2() {
  SHOW_SIZEOF_AND_ALIGNOF(char);
  SHOW_SIZEOF_AND_ALIGNOF(int);
  SHOW_SIZEOF_AND_ALIGNOF(long);
  SHOW_SIZEOF_AND_ALIGNOF(float);
  SHOW_SIZEOF_AND_ALIGNOF(double);
  SHOW_SIZEOF_AND_ALIGNOF(Foo);
  SHOW_SIZEOF_AND_ALIGNOF(Foo32);
  SHOW_SIZEOF_AND_ALIGNOF(Foo4);

  std::cout << "-------------------" << std::endl;
  std::cout << offsetof(Foo, c) << '\n'
            << offsetof(Foo, i1) << '\n'
            << offsetof(Foo, i2) << '\n'
            << offsetof(Foo, l) << '\n';

  return 0;
}

int func3() {
  char data[16];
  alignas(16) char aligned_data[16];
  std::cout << "unaligned: " << alignof(data)
            << ", aligned: " << alignof(aligned_data) << std::endl;
  return 0;
}

// alignas(Types...) unsigned char buffer[sizeof(LargestT)];
/**
 * // https://en.cppreference.com/w/cpp/language/alignas.html
 * alignas 展开规则：
      - 当 alignas 接受参数包时，实际生效的是参数包中所有对齐值的最严格对齐要求
      - 等同于 alignas(alignof(T1), alignof(T2), ...)，但最终取最大值
 */

struct alignas(4) A {
  char x;
};

struct alignas(16) B {
  double y;
};

template <typename... Types>
int func4() {
  alignas(Types...) char buffer[100];

  std::cout << "Alignment of buffer: " << alignof(buffer)
            << std::endl; // 输出16

  return 0;
}

int main(int argc, char const *argv[]) {
  // func1();
  // func2();
  // func3();
  func4<A, B>();
  return 0;
}

// keys: std::aligned_storage, std::align, alignof, alignas

// https://zhuanlan.zhihu.com/p/417061548
