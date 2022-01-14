#include <windows.h>
#include <conio.h>

typedef int (*fSquare)(int);
typedef int (*func)(int, int);

int main()
{
    HINSTANCE hInst;
    func pfnMy;
    fSquare pfnMyFunction;
    int iCode;
    while(!kbhit()) 
    {
        // char esc = getch();
        // if(esc == 27)
        // {
        //     break;
        // }
        if((hInst = LoadLibrary("lib1.dll")) == NULL) 
        {
            return 1;
        }
        printf("Load library.\n");
        Sleep(5000);
        // getch();
        // if(esc == 27)
        // {
        //     break;
        // }
        boolean fFreeResult;
        if (hInst != NULL)
        {
            fFreeResult = FreeLibrary(hInst);      
        }
        printf("Free library.\n");
        Sleep(6000);
    }

    // if((hInst = LoadLibrary("lib1")) == NULL) 
    // {
    //     return 1;
    // }
    // pfnMyFunction = (fSquare)GetProcAddress(hInst, "square");
    // iCode = (*pfnMyFunction)(3);
    // printf("%d\n", iCode);
        
    // pfnMy = (func)GetProcAddress(hInst, "add");
    // iCode = (*pfnMy)(3,5);
    // printf("%d\n", iCode);

    // pfnMy = (func)GetProcAddress(hInst, MAKEINTRESOURCE(3));
    // iCode = (*pfnMy)(5,3);
    // printf("%d\n", iCode);
    // return 0;
}