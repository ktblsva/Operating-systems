#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <locale.h>

int main(int argc, char* argv[])
{
	// for(int i=0; i<argc; i++)
	// {
	// 	printf("%s\n", argv[i]);
	// }
	// char* path = malloc(argc + 1);
	// for (int i = 0; i < argc; i++)
	// {
	// 	strcat(path, argv[i]);
	// }
    // printf("%s\n", path);
	FILE* file = fopen(argv[0], "a+");
	fclose(file);
	return 0;
}