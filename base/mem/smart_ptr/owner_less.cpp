// https://cplusplus.com/reference/memory/owner_less/
// https://en.cppreference.com/w/cpp/memory/owner_less

/**
 * This function object provides owner-based (as opposed to value-based)
 * mixed-type ordering of both std::weak_ptr and std::shared_ptr.
 * The order is such that two smart pointers compare equivalent only
 * if they are both empty or if they share ownership, even if the values
 * of the raw pointers obtained by get() are different
 * (e.g. because they point at different subobjects within the same object).
 */

// owner_less example
#include <iostream>
#include <memory>
#include <set>

int main() {
  int* p = new int(10);

  std::shared_ptr<int> a(new int(20));
  auto a1 = a;
  std::shared_ptr<int> b(a, p);  // alias constructor: co-owns a, points to p

  // standard set container: cannot contain duplicates.
  std::set<std::shared_ptr<int>> value_based;  // uses std::less
  std::set<std::shared_ptr<int>, std::owner_less<std::shared_ptr<int>>>
      owner_based;

  value_based.insert(a);
  value_based.insert(a1);
  value_based.insert(b);  // ok, different value

  owner_based.insert(a);
  owner_based.insert(a1);
  owner_based.insert(b);  // overwrites (same owned pointer)

  std::cout << "value_based.size() is " << value_based.size() << '\n';  // 2
  std::cout << "owner_based.size() is " << owner_based.size() << '\n';  // 1

  delete p;
  return 0;
}
