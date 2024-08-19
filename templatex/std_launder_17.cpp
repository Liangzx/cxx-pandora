#include <cassert>
#include <cstddef>
#include <iostream>
#include <new>

struct Y {
  int z;
};

struct A {
  virtual int transmogrify();
};

struct B : A {
  int transmogrify() override {
    new (this) A;
    return 2;
  }
};

int A::transmogrify() {
  new (this) B;
  return 1;
}

static_assert(sizeof(B) == sizeof(A));

void func1() {
  // 情况 1 ：新对象无法为透明可替换，因为它是基类子对象而旧对象是完整对象。
  A i;
  int n = i.transmogrify();
  // int m = i.transmogrify(); // 未定义行为
  int m = std::launder(&i)->transmogrify();  // OK
  assert(m + n == 3);

  // 情况 2 ：通过指向字节数组的指针访问存储为该数组所提供的新对象。
  alignas(Y) std::byte s[sizeof(Y)];
  Y *q = new (&s) Y{2};
  const int f =
      reinterpret_cast<Y *>(&s)->z;  // 类成员访问为未定义行为：
                                     // reinterpret_cast<Y*>(&s) 拥有值
                                     // “指向 s 的指针”而不指向 Y 对象
  const int g = q->z;                                        // OK
  const int h = std::launder(reinterpret_cast<Y *>(&s))->z;  // OK

  [f, g, h] {};  // 压制“未使用变量”警告；参阅 [[maybe_unused]] 。
}

// 对象模型：字节与对象的澄清
struct foo {
  std::uint32_t a;
  std::uint32_t b;
};

std::uint32_t bar(std::uint64_t &i, const foo &f) noexcept {
  if (f.a == 2) {
    i = 4;
  }

  if (f.a == 2) {
    return f.a;
  }

  return f.b;
}

std::uint32_t bar1(std::uint64_t *i, const foo &f) noexcept {
  if (f.a == 2) {
    *i = 4;
  }

  if (f.a == 2) {
    return f.a;
  }

  return f.b;
}

void func2() {
  foo f{2, 3};
  // gcc 9.4.0
  // g++ -std=c++17 templatex/std_launder_17.cpp -O3 ret=2
  // g++ -std=c++17 templatex/std_launder_17.cpp ret=0
  auto ret = bar(reinterpret_cast<std::uint64_t &>(f), f);
  std::cout << "ret: " << ret << std::endl;
}

void func3() {
  foo f{2, 3};
  auto pp = reinterpret_cast<std::uint64_t *>(&f);
  auto ret1 = bar1(pp, f);
  std::cout << "ret: " << pp << std::endl;
  std::cout << "ret: " << &f << std::endl;
  std::cout << "ret: " << ret1 << std::endl;
  std::cout << "pp: " << *pp << std::endl;
  std::cout << "f.a:" << f.a << std::endl;
  std::cout << "f.b:" << f.b << std::endl;
}

int main() {
  func3();
  return 0;
}

// https://www.bluepuni.com/archives/cpp-type-punning-and-implicit-lifetime/
// https://blog.csdn.net/fpcc/article/details/122489656
