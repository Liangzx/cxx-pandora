#include <cassert>
#include <iostream>
#include <vector>

/**
 * 表达式模板（Expression Templates） 是 C++ 中的一种高级模板编程技术，
 * 主要用于优化数值计算（如矩阵运算、向量运算等）的性能。它的核心思想是通过模板和运算符重载，
 * 将表达式延迟求值，从而避免不必要的临时对象创建和重复计算。
 *
 * 1. 表达式模板的背景
  在数值计算中，常常需要对大型数组、矩阵或向量进行复杂的运算。例如：
  Vector z = x + y + w;
  传统的实现方式会为每个运算符（如 +）创建一个临时对象，导致性能下降。例如：

  计算 x + y，生成一个临时对象 temp1。
  计算 temp1 + w，生成另一个临时对象 temp2。
  将 temp2 赋值给 z。
  这种方式会产生大量的临时对象，影响性能。
  表达式模板通过将表达式转换为模板表达式树，延迟求值，从而避免临时对象的创建。

  2. 表达式模板的核心思想
  表达式模板的核心思想是：
  将表达式（如 x + y + w）表示为一个模板表达式树。
  在表达式树的每个节点中，存储操作数和操作符，但不立即计算。
  在最终需要结果时，通过一次遍历计算整个表达式。

  https://blog.csdn.net/qq_40178082/article/details/144679064
 */

// CRTP中的基类模板
template <typename E>
class VecExpression {
 public:
  // 通过将自己static_cast成为子类，调用子类的对应函数实现实现静态多态
  double operator[](size_t i) const { return static_cast<E const&>(*this)[i]; }
  size_t size() const { return static_cast<E const&>(*this).size(); }
};

// 将自己作为基类模板参数的子类 - 对应表达式编译树中的叶节点
class Vec : public VecExpression<Vec> {
  std::vector<double> elems;

 public:
  double operator[](size_t i) const { return elems[i]; }
  double& operator[](size_t i) { return elems[i]; }
  size_t size() const { return elems.size(); }

  Vec(size_t n) : elems(n) {}

  Vec(std::initializer_list<double> init) {
    for (auto i : init) elems.push_back(i);
  }

  // 赋值构造函数可以接受任意父类VecExpression的实例，并且进行表达式的展开（对应表达式编译树中的赋值运算符节点）
  template <typename E>
  Vec(VecExpression<E> const& vec) : elems(vec.size()) {
    for (size_t i = 0; i != vec.size(); ++i) {
      elems[i] = vec[i];
    }
  }
};

// 将自己作为基类模板参数的子类 - 对应表达式编译树中的二元运算符输出的内部节点
// 该结构的巧妙之处在于模板参数E1 E2可以是VecSum，从而形成VecSum<VecSum<VecSum
// ... > >
// >的嵌套结构，体现了表达式模板的精髓：将表达式计算改造成为了构造嵌套结构
template <typename E1, typename E2>
class VecSum : public VecExpression<VecSum<E1, E2> > {
  E1 const& _u;
  E2 const& _v;

 public:
  VecSum(E1 const& u, E2 const& v) : _u(u), _v(v) {
    assert(u.size() == v.size());
  }

  // 延迟求值
  // 普通如v0 + v1在 + 的时候就求值了，而VecSum的+只是构造了表达式树，并没有求值
  // 在调用v3[i]的时候，才进行求值
  double operator[](size_t i) const { return _u[i] + _v[i]; }
  size_t size() const { return _v.size(); }
};

// 对应编译树上的二元运算符，将加法表达式构造为VecSum<VecSum... > >的嵌套结构
template <typename E1, typename E2>
VecSum<E1, E2> const operator+(const E1& u, const E2& v) {
  return VecSum<E1, E2>(u, v);
}

// 主函数入口
int main() {
  // 创建3个叶子节点
  Vec v0 = {1.0, 1.0, 1.0};
  Vec v1 = {2.0, 2.0, 2.0};
  Vec v2 = {3.0, 3.0, 3.0};

  // 构建表达式 v0 + v1 + v2，赋值给v3，编译阶段形成表达式树
  // VecSum<VecSum<Vec, Vec>, Vec> v3 = v0 + v1 + v2;
  // VecSum<Vec, Vec>[] -> _u[i] + _v[i]
  // VecSum<Vec, Vec>, Vec>[] -> (_u[i] + _v[i]) + elems[i]
  Vec v3 = v0 + v1 + v2;

  // 输出结算结果
  for (int i = 0; i < v3.size(); i++) {
    std::cout << " " << v3[i];
  }
  std::cout << std::endl;
}
