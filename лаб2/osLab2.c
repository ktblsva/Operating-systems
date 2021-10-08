#include <windows.h>
#include <stdio.h>
#include <math.h>
//cmd.exe /k "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat" `& powershell 
//cd  D:\"семестр 5"\os\"лаб2"
int main()
{
    char Buffer[100];
    int size = sizeof(Buffer);
    if (!GetTempPath((DWORD)size, (LPTSTR)Buffer))
    {
        printf("System error code: %i\n", GetLastError());
        return -1;
    }
    fprintf(stdout,"Temp path is %s\n", Buffer);
    if (!GetWindowsDirectory((LPTSTR)Buffer, (DWORD)size))
    {
        printf("System error code: %i\n", GetLastError());
        return -1;
    }
    fprintf(stdout,"Windows directory path is %s\n", Buffer);

    SYSTEM_INFO sSysInfo;
	GetSystemInfo(&sSysInfo);
    fprintf(stdout, "Number Of Processors: %d\n",  sSysInfo.dwNumberOfProcessors);
    fprintf(stdout, "Processor Type: %d\n",  sSysInfo.wProcessorArchitecture);
    fprintf(stdout, "Minimum Application Address: %lp\n",  sSysInfo.lpMinimumApplicationAddress);
    fprintf(stdout, "Maximum Application Address: %lp\n",  sSysInfo.lpMaximumApplicationAddress);
    
    MEMORYSTATUS sMem;
    GlobalMemoryStatus(&sMem);
    fprintf(stdout, "Total Physical: %.2f GB\n", sMem.dwTotalPhys / (1024.0 * 1024.0 * 1024.0));
    fprintf(stdout, "Avail Physical: %.2f GB\n", sMem.dwAvailPhys / (1024.0 * 1024.0 * 1024.0));

    fprintf(stdout, "Total Virtual: %.2f TB\n", sMem.dwTotalVirtual / (1024.0 * 1024.0 * 1024.0 * 1024.0));
    fprintf(stdout, "Avail Virtual: %g TB\n", sMem.dwAvailVirtual / (1024.0 * 1024.0 * 1024.0 * 1024.0));
    return 0;
} 
