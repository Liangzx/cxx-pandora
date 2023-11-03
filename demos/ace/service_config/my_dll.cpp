/// my_dll.cpp/
#include "ace/Service_Object.h"

#include <iostream>

class MyDynamicObj : public ACE_Service_Object {
public:
  MyDynamicObj(){};

  virtual ~MyDynamicObj(){};

  virtual int init(int argc, ACE_TCHAR *argv[]) {
    std::cout << "MyDynamicObj::init------" << std::endl;

    return 0;
  }

  virtual int suspend() {
    std::cout << "MyDynamicObj::suspend-----" << std::endl;

    return 0;
  }

  virtual int resume() {
    std::cout << "MyDynamicObj::resume-----" << std::endl;

    return 0;
  }

  virtual int fini() {
    std::cout << "MyDynamicObj::fini-----" << std::endl;

    return 0;
  }
};

// 此处也可以提供一个静态函数来生成MyDynamicObj的实例
// 需要使用到ACE_FACTORY_DEFINE这个宏
MyDynamicObj _make_MyDynamicObj;
