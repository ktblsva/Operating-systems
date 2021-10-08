#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <locale.h>

int main(int argc, char* argv[])
{
    if (remove(argv[0]) != 0)
    {
        printf("Error\n");
    }
    return 0;
}