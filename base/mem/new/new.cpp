#include <iostream>

class Foo {
public:
  Foo() { std::cout << "Foo()" << std::endl; }
  void *operator new(std::size_t size) {
    std::cout << "operator new" << std::endl;
    return malloc(size);
  }
  void *operator new(std::size_t size, void *ptr) {
    std::cout << "placement new" << std::endl;
    return ptr;
  }

  ~Foo() { std::cout << "Destructor called" << std::endl; }
};

int main() {
  Foo *m = new Foo;
  Foo *m2 = new (m) Foo; // 使用 placement new 在已分配的内存上构造对象
  delete m2;
  auto ni = new int;
  return 0;
}

/*
https://mp.weixin.qq.com/s?__biz=MzU4MDc5NTA0Mw==&mid=2247487626&idx=2&sn=0e1ca710c1cb523edf12e1e3dc9f49fe&chksm=fd503f46ca27b650c2b7e6c18010ed1c1ff1b83bf83a95f5841a5b300c550ad51a67b7835ef5&scene=178&cur_album_id=2303347039117688834#rd
https://elixir.bootlin.com/llvm/latest/source/libcxx/src/new.cpp
https://blog.csdn.net/Suk_god/article/details/123781449
*/