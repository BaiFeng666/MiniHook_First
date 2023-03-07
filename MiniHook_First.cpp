#include <Windows.h>
#include <iostream>
#include "minhook/minhook.h"
#pragma comment (lib, "MinHook.x86.lib")

//typedef int (WINAPI* fMessageBoxA)(HWND, LPCSTR, LPCSTR, UINT);
using fMessageBoxA = int(WINAPI*)(HWND, LPCSTR, LPCSTR, UINT);
fMessageBoxA pMessageBoxA = NULL;

using fPrintf = int(WINAPI*)(const char*, ...);
fPrintf pPrintf = NULL;

PVOID pMessageBoxAAddress;
PVOID pPrintfAddress;

int WINAPI MessageBoxAHooked(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
    LPCSTR lpMyText = "MessageBoxA被破解！";
    return pMessageBoxA(hWnd, lpMyText, lpCaption, uType);
}

int WINAPI PrintfHooked(const char* str, ...) 
{
    return pPrintf("Printf被破解！\r\n");
}

void SetupMessageBoxAHook()
{
    /*pMessageBoxAAddress = (LPVOID)MessageBoxA;

    if (MH_CreateHook(pMessageBoxAAddress, &MessageBoxAHooked, (PVOID*)&pMessageBoxA) != MH_OK)
        return;

    if (MH_EnableHook(pMessageBoxAAddress) != MH_OK)
        return;*/

    pPrintfAddress = (LPVOID)printf;

    if (MH_CreateHook(pPrintfAddress, &PrintfHooked, (PVOID*)&pPrintf) != MH_OK)
    {
        return;
    }

    if (MH_EnableHook(pPrintfAddress) != MH_OK)
        return;

    std::cout << "Hook start!\n";
}


void initHook()
{
    if (MH_Initialize() != MH_OK)
    {
        MessageBoxA(NULL, "Error initialize minhook", "alternative hack", MB_OK | MB_ICONERROR);
    }
}

void UnHook()
{
    /*MH_DisableHook((PVOID)MessageBoxA);
    MH_RemoveHook((PVOID)MessageBoxA);*/
    MH_DisableHook((PVOID)printf);
    MH_RemoveHook((PVOID)printf);
    MH_Uninitialize();

}

int main()
{
    //minhook的初始化
    initHook();

    //MessageBoxAHook
    SetupMessageBoxAHook();
    //测试是否hook成功
    //MessageBoxA(NULL, "box1", "box1", MB_OK);
    printf("test1 \r\n");
    //卸载hook
    UnHook();
    //MessageBoxA(NULL, "box2", "box2", MB_OK);
    printf("test2 \r\n");

    system("pause");
}