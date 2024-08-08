#include <bitset>
#include <iostream>
#include <vector>

// Container template
template <typename T>
class Container {
 public:
  void add(T item) { data.push_back(item); }

  void print() const {
    for (const auto &item : data) {
      std::cout << item << " ";
    }
    std::cout << std::endl;
  }

 private:
  std::vector<T> data;
};

// Wrapper class that uses the Container template
template <typename T>
class Wrapper {
 public:
  void wrap() {
    container.template add(42);  // 使用 .template 访问模板成员
    container.print();
  }

 private:
  Container<T> container;
};

// Template base class
template <typename T>
class Base {
 public:
  template <typename U>
  void print(U u) {
    std::cout << u << std::endl;
  }

  using Type = T;
};

// Derived class from Base
template <typename T>
class Derived : public Base<T> {
 public:
  template <typename U>
  void useBase(U &&u) {
    this->template print<U>(u);  // 使用 .template 访问模板成员
  }
};

// template guide 5.51
// 13.3.3 Dependent Names of Templates
// T is template
template <unsigned long N>
void printBitset(std::bitset<N> const &bs) {
  std::cout << bs.template to_string<char, std::char_traits<char>,
                                     std::allocator<char>>();
}

template <typename T>
class Shell {
 public:
  template <int N>
  class In {
   public:
    template <int M>
    class Deep {
     public:
      virtual void f() {}
    };
  };
};

template <typename T, int N>
class Weird {
 public:
  void case1(typename Shell<T>::template In<N>::template Deep<N> *p) {
    p->template Deep<N>().f();  // inhibit virtual call
  }
  void case2(typename Shell<T>::template In<N>::template Deep<N> &p) {
    p.template Deep<N>().f();  // inhibit virtual call
  }
};

template <typename T>
class BXT {
 public:
  using Mystery = T;
  template <typename U>
  struct Magic;
};

template <typename T>
class DXTT : private BXT<T> {
 public:
  using typename BXT<T>::Mystery;
  Mystery *p;  // would be a syntax error without the earlier typename
};

template <typename T>
class DXTM : private BXT<T> {
 public:
  template <typename U>
  using Magic = typename BXT<T>::template Magic<T>;  // Alias template
  Magic<T> *plink;                                   // OK
};

int main() {
  Wrapper<int> w;
  w.wrap();
  Derived<int> d;
  d.useBase("hello");

  std::bitset<8> b2(10);
  std::cout << b2.to_string() << std::endl;
  printBitset(b2);

  return 0;
}
