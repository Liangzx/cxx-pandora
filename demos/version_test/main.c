// main.c
#include <stdio.h>
#include <dlfcn.h>

extern int add(int, int);

int main() {
    // 主程序直接调用add
    printf("Main program computes: 2 + 3 = %d\n", add(2, 3));

    // 通过动态库调用add
    void* lib = dlopen("./libmylib.so", RTLD_LAZY);
    if (!lib) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return 1;
    }

    void (*do_math)() = dlsym(lib, "do_math");
    do_math();

    dlclose(lib);
    return 0;
}
