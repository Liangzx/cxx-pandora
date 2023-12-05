// https://blog.csdn.net/Leeds1993/article/details/54563918

#include <iostream>
#include <memory>

int DanglingPointerSolution() {
  // OLD, problem with dangling pointer
  // PROBLEM: ref will point to undefined data!

  int *ptr = new int(10);
  int *ref = ptr;
  delete ptr;

  // NEW
  // SOLUTION: check expired() or lock() to determine if pointer is valid

  // empty definition
  std::shared_ptr<int> sptr;

  // takes ownership of pointer
  sptr.reset(new int);
  *sptr = 10;

  // get pointer to data without taking ownership
  std::weak_ptr<int> weak1 = sptr;

  // deletes managed object, acquires new pointer
  sptr.reset(new int);
  *sptr = 5;

  // get pointer to new data without taking ownership
  std::weak_ptr<int> weak2 = sptr;

  // weak1 is expired!
  if (auto tmp = weak1.lock()) {
    std::cout << *tmp << '\n';
  } else {
    std::cout << "weak1 is expired\n";
  }

  // weak2 points to new data (5)
  if (auto tmp = weak2.lock()) {
    std::cout << *tmp << '\n';
  } else {
    std::cout << "weak2 is expired\n";
  }

  return 0;
}

/*************** dependency cycle ***********************************/
#include <iostream>
#include <memory>

using namespace std;

class Parent;
class Child;

typedef shared_ptr<Parent> parent_ptr;
typedef shared_ptr<Child> child_ptr;

class Parent {
public:
  ~Parent() { cout << "~Parent()" << endl; }

public:
  child_ptr children;
};

class Child {
public:
  ~Child() { cout << "~Child()" << endl; }

public:
  //   parent_ptr parent;
  weak_ptr<Parent> parent;
};

int DependencyCycleSolution() {
  parent_ptr father(new Parent);
  child_ptr son(new Child);

  // 父子互相引用
  father->children = son;
  son->parent = father;

  cout << father.use_count() << endl; // 引用计数为2
  cout << son.use_count() << endl;    // 引用计数为2

  return 0;
}

// shared_from_this
class D : public std::enable_shared_from_this<D> {
public:
  void func() {
    std::shared_ptr<D> p = shared_from_this();
    p->show();
  }
  void show() { std::cout << "hello d" << std::endl; }
};

/*
	1. shared_ptr 在赋值时会对 __weak_ptr 进行赋值
	2. 如果用了 enable_shared_from_this, 那么对象应该使用 shared_ptr 进行管理
**/

int SharedFromThis() {
  // bad
  //   {
  //     D d;
  //     d.func();
  //     /**
  //      * terminate called after throwing an instance of 'std::bad_weak_ptr'
  //      * what():  bad_weak_ptr
  //      * Aborted
  //      */
  //   }
  // ok
  {
    auto dd = make_shared<D>();
    dd->func();
  }

  return 0;
}

int main() {
  //   DanglingPointerSolution();
  //   DependencyCycleSolution();
  SharedFromThis();
  return 0;
}
