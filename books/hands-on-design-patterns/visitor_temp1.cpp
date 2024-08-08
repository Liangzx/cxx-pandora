#include <iostream>

template <typename... Types> class Visitor;

template <typename T> class Visitor<T> {
public:
  virtual void visit(T *t) = 0;
};

template <typename T, typename... Types>
class Visitor<T, Types...> : public Visitor<Types...> {
public:
  using Visitor<Types...>::visit;
  virtual void visit(T *t) = 0;
};

// 定义两种类型
struct NodeA {};
struct NodeB {};

// 定义一个访问者
class ConcreteVisitor : public Visitor<NodeA, NodeB> {
public:
  void visit(NodeA *node) override {
    std::cout << "Visiting NodeA" << std::endl;
  }

  void visit(NodeB *node) override {
    std::cout << "Visiting NodeB" << std::endl;
  }
};

// 定义一个节点基类
class Node {
public:
  virtual void accept(Visitor<Node> &visitor) = 0;
};

// 定义两种类型的节点
class NodeA : public Node {
public:
  void accept(Visitor<NodeA, NodeB> &visitor) override {
    visitor.visit(static_cast<NodeA *>(this));
  }
};

class NodeB : public Node {
public:
  void accept(Visitor<NodeA, NodeB> &visitor) override {
    visitor.visit(static_cast<NodeB *>(this));
  }
};

int main() {
  ConcreteVisitor visitor;
  NodeA nodeA;
  NodeB nodeB;

  nodeA.accept(visitor); // 输出: Visiting NodeA
  nodeB.accept(visitor); // 输出: Visiting NodeB

  return 0;
}
