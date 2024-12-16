#include <iostream>

#ifdef _WIN32
#ifdef __GNUC__
// For MinGW or Cygwin on Windows
#define CALL_CONV __attribute__((cdecl))
#else
// For MSVC on Windows
#define CALL_CONV __cdecl
#endif
#else
// For non-Windows platforms
#define CALL_CONV
#endif

void CALL_CONV test(int x, int y, int z) {
  std::cout << "x:" << &x << std::endl;
  std::cout << "y:" << &y << std::endl;
  std::cout << "z:" << &z << std::endl;
}

int main() {
  test(1, 2, 3);
  int x1 = 1;
  int y1 = 2;
  std::cout << "x1:" << &x1 << std::endl;
  std::cout << "y1:" << &y1 << std::endl;
  int arr[2]{1, 2};
  std::cout << "arr0:" << &arr[0] << std::endl;
  std::cout << "arr1:" << &arr[1] << std::endl;
  return 0;
}
