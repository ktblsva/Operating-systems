#include <windows.h>
#include <process.h>
#include <stdio.h>
//cmd.exe /k "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars32.bat" `& powershell 
//cl /MT /D "_X86_"
//visual c++
typedef int (*fun)(char*, int* p);
int g(char* str, int* p) {
    *p = 0;
    printf("%s\n", str);
    return 0;
}
int q=1;

void Thread(void* pg) {
    int counter = 0;
    while(q) {
        printf("child\n");
        Sleep(10);
        if(counter++ > 1000)
        {
            break;
        }
        ((fun)pg)("thread_is_over!", &q);
    }
}

int main(void) {
    _beginthread(Thread, 0, (void*)g);
    while(q) {
        printf("parent\n", q);
        Sleep(10);
    }
    return 0;
}