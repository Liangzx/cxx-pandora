// shared_ptr initialized with nullptr is null or empty?

#include <iostream>
#include <memory>

int main() {
  // Uses default constructor: shared_ptr();
  std::shared_ptr<int> p1;
  // Uses constructor: shared_ptr(std::nullptr_t);
  std::shared_ptr<int> p2(nullptr);

  // They are both null
  std::cout << std::boolalpha << !p1 << " " << !p2 << "\n";  // true true

  // Uses constructor: template<class Y>
  //                  explicit shared_ptr(Y* ptr);
  int* iptr{nullptr};
  // Both p1 and p2 are null, and they are empty,
  // p3 is null but not empty because it has a control block with
  // a managed nullptr and a reference count of 1
  std::shared_ptr<int> p3(iptr);

  // null check
  std::cout << std::boolalpha << !p3 << "\n";  // true
  std::cout << "-----------------" << std::endl;
  // ref count: 0
  std::cout << p1.use_count() << "\n";  // 0
  // ref count: 0
  std::cout << p2.use_count() << "\n";  // 0
  // ref count: 1
  std::cout << p3.use_count() << "\n";  // 1

  std::cout << "-----------------" << std::endl;
  // p1's (or p2's) copy is null and empty
  auto p1c = p1;
  // ref count: 0
  std::cout << p1c.use_count() << "\n";  // 0

  // p3 is different. ref count increases with copy.
  auto p3c = p3;
  // ref count: 2
  std::cout << p3c.use_count() << "\n";  // 2

  std::cout << "------------------" << std::endl;
  int x = 100;
  //'px' holds &x, but is empty.
  // A null and empty shared_ptr<void> is passed
  // to aliasing constructor to initialize px
  std::shared_ptr<int> px(std::shared_ptr<void>(), &x);
  std::cout << px.use_count() << std::endl;

  return 0;
}
