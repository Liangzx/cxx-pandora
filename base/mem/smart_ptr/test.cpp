#include <memory>
#include <iostream>

class A {
public:
  std::shared_ptr<A> SharedPtr() {
    auto sh = std::shared_ptr<A>(this);
    std::cout << sh.use_count() << std::endl; // 1
    // 返回时 this 已经释放
    return sh;
  }
  void Show() { std::cout << "Hello!" << std::endl; }
};

//////////////////////////////////////////////////////////////
#include <iostream>
#include <memory>

class Test : public std::enable_shared_from_this<Test> // 改进1
{
public:
  // 析构函数
  ~Test() { std::cout << "Test Destructor." << std::endl; }
  // 获取指向当前对象的指针
  std::shared_ptr<Test> GetObject() {
    return shared_from_this(); // 改进2
  }
};

int main() {

  //   A a;
  //   auto as = a.SharedPtr();
  //   std::cout << as.use_count() << std::endl; // 1
  //   as->Show();
  {
    std::shared_ptr<Test> p(new Test());
    std::cout << "p.use_count(): " << p.use_count() << std::endl; // 1
    std::shared_ptr<Test> q = p->GetObject();
    std::cout << "p.use_count(): " << p.use_count() << std::endl; // 2
    std::cout << "q.use_count(): " << q.use_count() << std::endl; // 2
  }
  return 0;
}
