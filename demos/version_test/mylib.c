// mylib.c
#include <stdio.h>

extern int add(int, int);  // 声明外部add函数

void do_math() {
    int result = add(2, 3);
    printf("Dynamic lib computes: 2 + 3 = %d\n", result);
}
