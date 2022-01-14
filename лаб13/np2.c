#include <stdio.h>
#include <windows.h>

void main()
{
    HANDLE hPipe;
    LPTSTR lpPipeName = TEXT("\\\\.\\pipe\\myPipe"); //TEXT("\\\\DESKTOP-FQ5V6LA.sib.mts.ru\\pipe\\MyPipe");
    DWORD iBytesToWrite;
    char buff[]="A message from client";
    hPipe = CreateFile(lpPipeName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    WriteFile(hPipe, buff, strlen(buff), &iBytesToWrite, NULL);
    CloseHandle(hPipe);
}