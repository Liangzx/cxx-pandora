#include <exception>
#include <iostream>
#include <signal.h>
#include <string>

template<typename T> T f (T p);
decltype(f(2)) x = 2;

int main() {
  decltype(f(2)) x = 2;
  return 0;
}
