// process2.c
#include <dlfcn.h>
#include <stdio.h>

typedef int (*GetGlobalVarFunc)();
typedef int* (*GetGlobalVarPtrFunc)();
typedef void (*IncrementGlobalVarFunc)();

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

size_t virtual_to_physical(size_t addr) {
  int fd = open("/proc/self/pagemap", O_RDONLY);
  if (fd < 0) {
    printf("open '/proc/self/pagemap' failed!\n");
    return 0;
  }
  size_t pagesize = getpagesize();
  size_t offset = (addr / pagesize) * sizeof(uint64_t);
  if (lseek(fd, offset, SEEK_SET) < 0) {
    printf("lseek() failed!\n");
    close(fd);
    return 0;
  }
  uint64_t info;
  if (read(fd, &info, sizeof(uint64_t)) != sizeof(uint64_t)) {
    printf("read() failed!\n");
    close(fd);
    return 0;
  }
  if ((info & (((uint64_t)1) << 63)) == 0) {
    printf("page is not present!\n");
    close(fd);
    return 0;
  }
  size_t frame = info & ((((uint64_t)1) << 55) - 1);
  size_t phy = frame * pagesize + addr % pagesize;
  close(fd);
  return phy;
}

int main() {
  void* handle = dlopen("./libexample.so", RTLD_LAZY);
  IncrementGlobalVarFunc increment_global_var =
      (IncrementGlobalVarFunc)dlsym(handle, "increment_global_var");

  GetGlobalVarFunc get_global_var =
      (GetGlobalVarFunc)dlsym(handle, "get_global_var");

  GetGlobalVarPtrFunc get_global_var_ptr =
      (GetGlobalVarPtrFunc)dlsym(handle, "get_global_var_ptr");

  increment_global_var();
  printf(
      "Process 1: global_var = %d, global_var_ptr=%p global_var_ptr_pys=%p\n",
      get_global_var(), get_global_var_ptr(),
      virtual_to_physical((size_t)get_global_var_ptr()));

  dlclose(handle);
  return 0;
}

// gcc -o proc2 proc2.c -ldl
