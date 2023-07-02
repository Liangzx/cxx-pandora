#include <vector>
#include <iostream>

<<<<<<< HEAD
void pptr(int *a) {

}

=======
>>>>>>> 678a107c9d7061c09a75ef1cd479c4dcf2ff44ff
int main() {
  std::vector<int> a{1, 3, 5};
  std::vector<int> b{1, 3, 5};
  if (a == b) {
    std::cout << "a == b" << std::endl;
  }
  return 0;
}