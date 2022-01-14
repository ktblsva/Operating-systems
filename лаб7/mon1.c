#include <windows.h>
#include "Psapi.h"

void PrintProcessModules(DWORD pID)
{
    HMODULE modHndls[1024];
    HANDLE pHndle;
    DWORD cbNeeded;
    unsigned int i;
    printf("\nProcess ID: %u\n", pID);
    pHndle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                    FALSE, pID);
    if (NULL == pHndle)
        return;
    if(EnumProcessModules(pHndle, modHndls, sizeof(modHndls), &cbNeeded))
    {
        for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
        {
            char szModName[MAX_PATH];
            GetModuleBaseName(pHndle, modHndls[i], (LPSTR)szModName, sizeof(szModName));
            printf("%u\t%s", modHndls[i],szModName);
            if (GetModuleFileNameEx(pHndle, modHndls[i], szModName, sizeof(szModName)))
            {
                printf("\t%s (0x%08X)\n", szModName, modHndls[i]);
            }
        }
    }
    CloseHandle( pHndle );
}

void main( )
{
    SetConsoleOutputCP(1251);
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        return;

    cProcesses = cbNeeded / sizeof(DWORD);
    PrintProcessModules(aProcesses[cProcesses - 2]);
    printf("\n");
}