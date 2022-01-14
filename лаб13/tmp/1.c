//ноут 192.168.1.253 //DESKTOP-FQ5V6LA
#include <stdio.h>
#include <windows.h>

int main() {
    HANDLE hPipe1;
    HANDLE hPipe2;
    LPTSTR lpPipeName1 = TEXT("\\\\.\\pipe\\MyPipe1");
    LPTSTR lpPipeName2 = TEXT("\\\\.\\pipe\\MyPipe2");

    char buff1[255] = " ";
    char buff2[255] = " ";
    DWORD iBytesToRead = 255;
    DWORD iBytesToWrite;
    
    hPipe1 = CreateNamedPipe(lpPipeName1, 
                            PIPE_ACCESS_DUPLEX, 
                            PIPE_TYPE_MESSAGE | 
                            PIPE_READMODE_MESSAGE | 
                            PIPE_WAIT, 
                            PIPE_UNLIMITED_INSTANCES, 
                            4096,
                            4096, 
                            NMPWAIT_USE_DEFAULT_WAIT,
                            NULL);
    
    if (hPipe1 == INVALID_HANDLE_VALUE) {
        printf("CreatePipe failed: error code %d\n", (int)GetLastError());    
        return 0;
    }

    if((ConnectNamedPipe(hPipe1, NULL)) == 0) {
        printf("client could not connect\n");
        return 0;
    }

    hPipe2 = CreateFile(lpPipeName2, 
                       GENERIC_READ | 
                       GENERIC_WRITE,
                       0, 
                       NULL,
                       OPEN_EXISTING,
                       0, 
                       NULL);

    while (1) {
        iBytesToWrite = 255;
        iBytesToRead = 255;
        printf("          ");
        ReadFile(hPipe1, buff1, iBytesToRead, &iBytesToRead, NULL);
        for(int i = 0; i < iBytesToRead; i++) printf("%c",buff1[i]);

        char c = getchar();
        int i = 0;
        while (c != '\n') {
            buff2[i] = c;
            i++;
            c = getchar();
        }
        
        buff2[i] = '\n';
        buff2[i + 1] = '\0';
        WriteFile(hPipe2, buff2, strlen(buff2), &iBytesToWrite, NULL);
    }

    CloseHandle(hPipe1);
    CloseHandle(hPipe2);
    
    return 0;
}