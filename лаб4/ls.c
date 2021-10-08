#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <locale.h>

int main(int argc, char* argv[])
{
	setlocale(LC_ALL,"Russian"); 
	char buffer[255];
	sprintf(buffer, "dir \"%s\"", argv[0]);
	system(buffer);
	return 0;
}