#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <locale.h>

#define N 255

int main(int argc, char* argv[])
{
	setlocale(LC_ALL,"Russian");
	FILE *file;
    char c;
	char fileName[255];
	sprintf(fileName, "%s\\help.txt", argv[0]);
	//printf("%s\n", fileName);
    file = fopen(fileName, "r");
	if (file == NULL)
	{
		return -1;
	}
	c = fgetc(file);
	while (c != EOF)
	{
		printf("%c",c);
		c = fgetc(file);
	}
 
    printf("\n");
    fclose(file);
	return 0;
}