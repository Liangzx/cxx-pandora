// https://stackoverflow.com/questions/1688290/creating-a-thread-in-dllmain

#include "MyDll.h"
#include <stdio.h>
#include <windows.h>

typedef int (*AddNumbersFunc)(int, int);

int main()
{
  HMODULE hDLL = LoadLibrary(TEXT("MyDll.dll"));
  if (hDLL != NULL) {
    // 获取函数指针
    AddNumbersFunc AddNumbers =
      (AddNumbersFunc)GetProcAddress(hDLL, "AddNumbers");

    if (AddNumbers != NULL) {
      // 调用DLL中的函数
      int result = AddNumbers(5, 3);
      printf("Result of AddNumbers(5, 3): %d\n", result);
    } else {
      printf("GetProcAddress failed. Error code: %d\n", GetLastError());
    }

    // 卸载DLL
    FreeLibrary(hDLL);
  } else {
    printf("LoadLibrary failed. Error code: %d\n", GetLastError());
  }

  return 0;
}
