#include <windows.h>
#include <stdio.h>
#pragma comment(lib, "libevd1")
HANDLE hSemaphore;
extern __declspec(dllimport)
char sh[6];
int main(void)
{
    hSemaphore = CreateSemaphore(NULL, 1, 1, "MySemaphore");
    while(1)
    {
        WaitForSingleObject(hSemaphore, INFINITE);
        printf("sh: %s\n", sh);
        ReleaseSemaphore(hSemaphore, 1, NULL);
        Sleep(100);
    }
    CloseHandle(hSemaphore);
    return 0;
}