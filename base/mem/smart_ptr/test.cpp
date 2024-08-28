#include <memory>
#include <iostream>

#include <iostream>
#include <boost/shared_ptr.hpp>
class Test
{
public:
    //析构函数
    ~Test() { std::cout << "Test Destructor." << std::endl; }
    //获取指向当前对象的指针
    std::shared_ptr<Test> GetObject()
    {
        std::shared_ptr<Test> pTest(this);
        return pTest;
    }
};

//////////////////////////////////////////////////////////////
#include <iostream>
#include <memory>

// enable_shared_from_this用法分析
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

  {
    std::shared_ptr<Test> p(new Test());
    std::cout << "p.use_count(): " << p.use_count() << std::endl; // 1
    std::shared_ptr<Test> q = p->GetObject();
    std::cout << "p.use_count(): " << p.use_count() << std::endl; // 2
    std::cout << "q.use_count(): " << q.use_count() << std::endl; // 2
  }
  return 0;
}
