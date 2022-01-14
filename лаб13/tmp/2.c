//DESKTOP-H5I15MN //192.168.1.253
#include <stdio.h>
#include <windows.h>
#include <string.h>

int main() {
    HANDLE hPipe1;
    HANDLE hPipe2;
    LPTSTR lpPipeName1 = TEXT("\\\\.\\pipe\\MyPipe1");
    LPTSTR lpPipeName2 = TEXT("\\\\.\\pipe\\MyPipe2");

    char buff1[255] = " ";
    char buff2[255] = " ";
    DWORD iBytesToRead = 255;
    DWORD iBytesToWrite;

    hPipe1 = CreateFile(lpPipeName1, 
                       GENERIC_READ | 
                       GENERIC_WRITE,
                       0, 
                       NULL, 
                       OPEN_EXISTING, 
                       0,
                       NULL); 

    hPipe2 = CreateNamedPipe(lpPipeName2,
                            PIPE_ACCESS_DUPLEX, 
                            PIPE_TYPE_MESSAGE |
                            PIPE_READMODE_MESSAGE |
                            PIPE_WAIT,
                            PIPE_UNLIMITED_INSTANCES,
                            4096, 
                            4096, 
                            NMPWAIT_USE_DEFAULT_WAIT,
                            NULL); 
    
    if (hPipe2 == INVALID_HANDLE_VALUE) {
        printf("CreatePipe failed: error code %d\n", (int)GetLastError());    
        return 0;
    }

    if((ConnectNamedPipe(hPipe2, NULL)) == 0) {
        printf("client could not connect\n");
        return 0;
    }
    
    while (strcmp(buff1, "q")) {
        iBytesToWrite = 255;
        iBytesToRead = 255;

        char c = getchar();
        int i = 0;
        while (c != '\n') {
            buff1[i] = c;
            i++;
            c = getchar();
        }
        
        buff1[i] = '\n';
        buff1[i + 1] = '\0';
        WriteFile(hPipe1, buff1, strlen(buff1), &iBytesToWrite, NULL);

        ReadFile(hPipe2, buff2, iBytesToRead, &iBytesToRead, NULL);
        printf("          ");
        for(i = 0; i < iBytesToRead; i++) printf("%c",buff2[i]);
    }

    CloseHandle(hPipe1);
    CloseHandle(hPipe2);
    
    return 0;
}