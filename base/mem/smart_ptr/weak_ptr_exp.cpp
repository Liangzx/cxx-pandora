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

// https://juejin.cn/post/7137133849430982693
void func1() {
  weak_ptr<int> wp;
  {
    shared_ptr<int> sp(new int(1));  // sp.use_count()==1
    std::cout << sp.use_count() << std::endl;
    wp = sp;  // wp不会改变引用计数，所以sp.use_count()==1
    std::cout << sp.use_count() << std::endl;
    shared_ptr<int> sp_ok =
        wp.lock();  // wp没有重载->操作符。只能这样取所指向的对象
    // sp.use_count()==2
    std::cout << sp.use_count() << std::endl;
  }

  if (auto pp1 = wp.lock()) {
    std::cout << "-----------" << std::endl;
    std::cout << *pp1 << std::endl;
  }

  shared_ptr<int> sp_null = wp.lock();  // sp_null .use_count()==0;
  std::cout << sp_null.use_count() << std::endl;
  if (!wp.expired()) {
    std::cout << "执行流程" << endl;
  } else {
    std::cout << "已经销毁" << endl;
  }
  // Segmentation fault (core dumped)
  // std::cout << *sp_null << std::endl;
}

void func2() {
  shared_ptr<int> sp(new int(1));  // sp.use_count()==1
  weak_ptr<int> wp = sp;
  wp = sp;  // wp不会改变引用计数，所以sp.use_count()==1
  std::cout << sp.use_count() << std::endl;
  if (!wp.expired()) {  // 1 != 0 true
    // 此时其他线程将 sp 释放，则 sp.use_count()==0
    shared_ptr<int> sp_ok = wp.lock();  // sp.use_count()==0，无效
    std::cout << sp.use_count() << std::endl;
    // 再对指针操作都是无效操作了
  } else {
    std::cout << "已经销毁" << std::endl;
  }

  if (auto pp1 = wp.lock()) {
    std::cout << *pp1 << std::endl;
  }
}

int main() {
  //   DanglingPointerSolution();
  //   DependencyCycleSolution();
  // SharedFromThis();
  func1();
  // func2();
  return 0;
}
