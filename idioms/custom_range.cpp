// https://www.cnblogs.com/h46incon/archive/2013/06/02/3113737.html
// https://blog.csdn.net/haokan123456789/article/details/138561545
// https://www.boost.org/doc/libs/1_47_0/boost/range/adaptor/transformed.hpp

#include <iostream>
using namespace std;
template <typename T, size_t N>
class A {
 public:
  A() {
    for (size_t i = 0; i < N; ++i) {
      m_elements[i] = i;
    }
  }
  ~A() {}
  T* begin() { return m_elements + 0; }
  T* end() { return m_elements + N; }

 private:
  T m_elements[N];
};

//-------
#include <iostream>

// 自定义迭代器类
class IntegerIterator {
 public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = int;
  using pointer = int*;
  using reference = int&;

  IntegerIterator(int value) : current(value) {}

  // 前缀递增
  IntegerIterator& operator++() {
    ++current;
    return *this;
  }

  // 后缀递增（不常用，但为了满足迭代器的概念）
  IntegerIterator operator++(int) {
    IntegerIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  // 解引用
  int operator*() const { return current; }

  // 相等比较
  bool operator==(const IntegerIterator& other) const {
    return current == other.current;
  }

  // 不相等比较
  bool operator!=(const IntegerIterator& other) const {
    return !(*this == other);
  }

 private:
  int current;
};

// 自定义整数范围类
class IntegerRange {
 public:
  IntegerRange(int start, int end) : start_(start), end_(end) {}

  // 提供begin迭代器
  IntegerIterator begin() const { return IntegerIterator(start_); }

  // 提供end迭代器
  IntegerIterator end() const { return IntegerIterator(end_); }

 private:
  int start_;
  int end_;
};

int main() {
  IntegerRange range(1, 5);

  // 使用range-based for循环遍历range
  for (int num : range) {
    std::cout << num << ' ';
  }

  std::cout << '\n';

  return 0;
}

// int main() {
//   A<int, 10> a;
//   for (auto iter : a) {
//     std::cout << iter << endl;
//   }
// }
