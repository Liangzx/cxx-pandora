#include "student.h"

#include <iostream>

Student::Student() {
  a_ = 11;
  b_ = 12;
  c_ = 13;
}
void Student::func1() { std::cout << "func1" << std::endl; }
void Student::func2() { std::cout << "func2" << std::endl; }
void Student::printa() { std::cout << this->a_ << std::endl; }