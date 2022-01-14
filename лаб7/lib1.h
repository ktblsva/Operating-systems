#pragma once
#define DllExport __declspec(dllexport)
DllExport int square(int a);
DllExport int add(int a, int b);
DllExport int sub(int a, int b);