#include <windows.h>
#include <iostream>

typedef int (*MYPROC)(LPWSTR);

int main()
{
    HINSTANCE hinstLib;
    BOOL fFreeResult;

    hinstLib = LoadLibrary(TEXT("lib1.dll"));    

    if (hinstLib != NULL)
    {
    fFreeResult = FreeLibrary(hinstLib);      
    }
    return 0;
}