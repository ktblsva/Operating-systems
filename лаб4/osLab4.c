#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <locale.h>

#define BUFSIZE MAX_PATH
//cmd.exe /k "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat" `& powershell 
//cd  D:\"семестр 5"\os\"лаб4"
TCHAR commandPath[MAX_PATH];
TCHAR currentPath[MAX_PATH];
const int commandsCount = 10;
const char* commands[] = {
	"clear",
	"ls",
	"cd",
	"touch",
	"pwd",
	"who",
	"help",
	"exit",
	"rm",
	"ifconfig"
};

void printCurrentPath()
{
	GetCurrentDirectory(BUFSIZE, currentPath);
	printf("lab4 %s> ", currentPath);
}

void commandHandler(char* command)
{
	char basicCommand[255];
	strcpy(basicCommand, command);
	char* arg = strtok(command, " ");
	int commandIndex = -1;
	for (int i = 0; i < commandsCount; i++)
	{
		if (strcmp(arg, commands[i]) == 0)
		{
			commandIndex = i;
			break;
		}
	}
	
	TCHAR commandArg[MAX_PATH];
	sprintf(commandArg, "%s\\%s", commandPath, arg);
	TCHAR tmp[255];
	TCHAR currentAbsPath[255];
	TCHAR commandAbsPath[255];
	sprintf(currentAbsPath, "\"%s\"", currentPath);
	sprintf(commandAbsPath, "\"%s\"", commandPath);
	switch (commandIndex)
	{	
		case(0): // clear			
			_spawnl(_P_WAIT, commandArg, "clear", NULL);
			break;
		case(1): // ls	
			command = strtok(NULL,"");
			if (command == NULL)
			{
				_spawnl(_P_WAIT, commandArg, currentAbsPath, NULL);
			}
			else
			{
				_spawnl(_P_WAIT, commandArg, command, NULL);
			}
			break;
		case(2): // cd
			TCHAR* tmp;
			tmp = strtok(NULL, "");
			// printf("%u\n", GetConsoleCP());
			if (SetCurrentDirectory(tmp) == 0)
			{
				printf("Wrong path.\n");
			}
			
			break;
		case(3): // touch
			command = strtok(NULL,"");
			if (command == NULL)
			{
				printf("There are too few arguments for this command.");
			}
			else
			{
				GetCurrentDirectory(BUFSIZE, currentPath);
				char fileName[255];
				sprintf(fileName, "\"%s\\%s\"", currentPath, command);			
				_spawnl(_P_WAIT, commandArg, fileName,NULL);
			}
			break;
		case(4): // pwd
			_spawnl(_P_WAIT, commandArg, currentAbsPath, NULL);
			break;
		case(5): // who
			_spawnl(_P_WAIT, commandArg, "who", NULL);
			break;
		case(6): // help
			//printf("%s\n", commandPath);
			_spawnl(_P_WAIT, commandArg, commandAbsPath, NULL);
			break;
		case(7):
			SetConsoleCP(866);
    		SetConsoleOutputCP(866);
			exit(0);
		case(8): // rm
			command = strtok(NULL,"");
			if (command == NULL)
			{
				printf("There are too few arguments for this command.");
			}
			else
			{
				GetCurrentDirectory(BUFSIZE, currentPath);
				char fileName[255];
				sprintf(fileName, "\"%s\\%s\"", currentPath, command);			
				_spawnl(_P_WAIT,commandArg,fileName,NULL);
			}
			break;
		case(10): // ifconfig
			_spawnl(_P_WAIT, commandArg, "ifconfig", NULL);
			break;
		default:
			system(basicCommand);
			break;
	}
	
}

int main()
{
	GetCurrentDirectory(BUFSIZE,commandPath);
	setlocale(LC_ALL,"Russian");
	system("cls");
	SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
	while (1)
	{
		char command[255];
		printCurrentPath();
		gets(&command);
		commandHandler(command);
	}
	
	return 0;
}