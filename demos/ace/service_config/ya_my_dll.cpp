// ya_my_dll.cpp/
#include "ace/Service_Object.h"

#include <iostream>

class YaMyDynamicObj : public ACE_Service_Object {
public:
  YaMyDynamicObj(){};

  virtual ~YaMyDynamicObj(){};

  virtual int init(int argc, ACE_TCHAR *argv[]) {
    std::cout << "Yet another YaMyDynamicObj::init------"<< std::endl;

    return 0;
  }

  virtual int suspend() {
    std::cout << "Yet another YaMyDynamicObj::suspend-----"<< std::endl;

    return 0;
  }

  virtual int resume() {
    std::cout << "Yet another YaMyDynamicObj::resume-----"<< std::endl;

    return 0;
  }

  virtual int fini() {
    std::cout << "Yet another YaMyDynamicObj::fini-----"<< std::endl;

    return 0;
  }
};

YaMyDynamicObj _make_YaMyDynamicObj;
