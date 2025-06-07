#include <iostream>
#include <execinfo.h> // for backtrace
#include <unistd.h>   // for STDERR_FILENO

void print_stacktrace() {
    void *buffer[100]; // 存储堆栈地址
    int nptrs = backtrace(buffer, 100); // 获取当前堆栈
    char **strings = backtrace_symbols(buffer, nptrs); // 转换为可读字符串
    if (strings == nullptr) {
        perror("backtrace_symbols");
        return;
    }

    // 打印堆栈信息
    for (int i = 0; i < nptrs; i++) {
        std::cerr << strings[i] << std::endl;
    }

    free(strings); // 释放内存
}

void foo() {
    print_stacktrace();
}

void bar() {
    foo();
}

int main() {
    bar();
    return 0;
}
