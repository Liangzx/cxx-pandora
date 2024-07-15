#include <stdio.h>
#include <stdlib.h>

void func2(){
    printf("☠️☠️☠️☠️☠️");
    exit(4);
}

void func1(){
    long a[2];
    a[0] = 1;
    a[1] = 2;
    // 修改返回地址
    a[3] = (long)func2;
}

int main(){
    func1();
    printf("hello");
}

// [CPU眼里的：函数调用 | 返回](https://mp.weixin.qq.com/s/dJw7UGxBbylNvQ4mzHjuEg)
// Ps: ubuntu2204 clang 可以验证但是gcc不能
