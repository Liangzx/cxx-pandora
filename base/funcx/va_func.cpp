/**
 * [理解可变参数va_list、va_start、va_arg、va_end原理及使用方法](https://www.cnblogs.com/pengdonglin137/p/3345911.html)
 * [va_start](https://en.cppreference.com/w/c/variadic/va_start)
 */

#include <stdarg.h>
#include <stdio.h>

int add_nums_C99(int count, ...) {
  int result = 0;
  va_list args;
  va_start(args, count);  // count can be omitted since C23

  for (int i = 0; i < count; ++i) {
    result += va_arg(args, int);
  }

  va_end(args);
  return result;
}

#if __STDC_VERSION__ > 201710L
// Same as above, valid since C23
int add_nums_C23(...) {
  int result = 0;
  va_list args;
  va_start(args);

  int count = va_arg(args, int);
  for (int i = 0; i < count; ++i) {
    result += va_arg(args, int);
  }

  va_end(args);
  return result;
}
#endif

int main(void) {
  printf("%d\n", add_nums_C99(4, 25, 25, 50, 50));
#if __STDC_VERSION__ > 201710L
  printf("%d\n", add_nums_C23(4, 25, 25, 50, 50));
#endif
}
