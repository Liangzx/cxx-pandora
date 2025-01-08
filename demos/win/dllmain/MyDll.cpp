#include "MyDll.h"
#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // 当进程加载 DLL 时调用
        OutputDebugStringA("MyDll: DLL_PROCESS_ATTACH\n");
        break;
    case DLL_THREAD_ATTACH:
        // 当进程中的新线程开始时调用
        OutputDebugStringA("MyDll: DLL_THREAD_ATTACH\n");
        break;
    case DLL_THREAD_DETACH:
        // 当进程中的线程结束时调用
        OutputDebugStringA("MyDll: DLL_THREAD_DETACH\n");
        break;
    case DLL_PROCESS_DETACH:
        // 当进程卸载 DLL 时调用
        OutputDebugStringA("MyDll: DLL_PROCESS_DETACH\n");
        break;
    }
    return TRUE; // 返回 TRUE 表示成功
}

/**
 * DllMain 针对单个的 dll
 * 如果其他 DLL 没有定义 DllMain 函数，那么在这些 DLL 被加载或卸载时，Windows 操作系统不会调用任何特殊的初始化或清理代码。具体来说：
 * DLL 加载：当一个没有 DllMain 的 DLL 被加载时，操作系统会完成加载过程，但不会执行任何特定于该 DLL 的初始化代码。这意味着，如果你的 DLL 依赖于另一个没有 DllMain 的 DLL，那么这个依赖的 DLL 将被正常加载，但它的全局构造函数（如果有）会在加载过程中被执行。
 * DLL 卸载：同样地，当一个没有 DllMain 的 DLL 被卸载时，操作系统会直接进行卸载操作，而不会调用任何清理代码。任何需要的清理工作（如释放资源、注销回调等）必须由应用程序或库本身通过其他方式处理。
 */

__declspec(dllexport) int AddNumbers(int a, int b)
{
    return a + b;
}
