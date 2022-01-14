#include <windows.h>
#define DllExport __declspec(dllexport)
#include "lib1.h"
int square(int a)
{
    return a * a;
}

int add(int a, int b)
{
    return a+b;
}

int sub(int a, int b)
{
    return a-b;
}