#include <iostream>

class HeapOnlyObject {
public:
  HeapOnlyObject(){};
  void Destroy() { delete this; }

private:
  ~HeapOnlyObject(){};
};

class HeapOnlyObject1 {
public:
  static HeapOnlyObject1 *Create() { return new HeapOnlyObject1{}; }
  void Destroy() { delete this; }

protected:
  HeapOnlyObject1(){};
};

/////////////////////////////////////////

class StackOnlyObject {

public:
  void *operator new(size_t t) = delete;
  void operator delete(void *ptr) = delete;
};

////////////////////////////////////////
class AbstractClassWithProtect {
protected:
  AbstractClassWithProtect(){};
};

int main() {
  // HeapOnlyObject *hoo = new HeapOnlyObject{};
  // HeapOnlyObject1 hoo1;
  // auto soo = new StackOnlyObject{};
  StackOnlyObject soo{};
  // AbstractClassWithProtect acwp{};
  return 0;
}