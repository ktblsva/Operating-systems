#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <locale.h>

int main(int argc, char* argv[])
{
	setlocale(LC_ALL,"RUSSIAN"); 
	// for(int i = 0; i < argc; i++)
	// {
	// 	printf("%s",argv[i]);
	// }
	printf("%s\n", argv[0]);
	return 0;
}