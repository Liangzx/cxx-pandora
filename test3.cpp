#include <iostream>

template <typename X> class Base {
public:
  int basefield;
  using T = int;
};

template <typename T> class DD : public Base<T> { // dependent base
public:
  void f() { Base<T>::basefield = 0; } // #1 problem...
};

template <> // explicit specialization
class Base<bool> {
public:
  // enum { basefield = 42 }; // #2 tricky!
};

void g(DD<bool> &d) {
  d.f(); // #3 oops?
}

int main() { return 0; }
