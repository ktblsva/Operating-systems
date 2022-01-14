#include <windows.h>
#include <stdio.h>
void main( void )
{
	STARTUPINFO si[2];
	PROCESS_INFORMATION pi[2];
	char* exefile=
	{
		"C:\\Windows\\System32\\mspaint.exe"
		//"C:\\Windows\\write.exe"
	};
	char* cmdLine="cmd /C C:\\Windows\\SysWOW64\\notepad.exe 1.cpp";
	HANDLE Handles[2];
		for(int i=0;i<1;i++)
		{
			ZeroMemory( &si[i], sizeof(si[i]) );
			si[i].cb = sizeof(si[i]);
			ZeroMemory( &pi[i], sizeof(pi[i]) );
		}
	for(int i=0;i<1;i++)
	{
		if (!CreateProcess
		(
			exefile[i], //"C:\\Windows\\notepad.exe", //Исполняемый модуль
			NULL, // Командная строка не используется
			NULL, // Дескриптор процесса не наследуется
			NULL, // Дескриптор потока не наследуется
			FALSE,
			// Открытые дескрипторы родительского процесса не //наследуется
			CREATE_NO_WINDOW,
			// Не создавать окна (консольного окна)
			NULL,
			// Используются переменные окружения родителя
			NULL, // Используется текущая директория родителя
			&si[i], // Указатель на структуру STARTUPINFO
			&pi[i] )
			// Указатель на структуру PROCESS_INFORMATION
		)
		{ 
			printf("System error code: %i\n",GetLastError()); 
		}
		
		Handles[i]=pi[i].hProcess;
	 }
	 
	if (!CreateProcess
	(
		NULL,
		cmdLine, // Используется командная строка
		NULL,
		NULL,
		FALSE,
		CREATE_NO_WINDOW,
		NULL,
		NULL,
		&si[1],
		&pi[1] 
	))
	{ 
		printf("System error code: %i\n",GetLastError()); 
	}
	Handles[1]=pi[1].hProcess;
	
	WaitForMultipleObjects(2, Handles, TRUE, INFINITE);
	
	printf("Go home!");
	for(int i=0;i<2;i++)
	{
		CloseHandle( pi[i].hProcess );
		CloseHandle( pi[i].hThread );
	}
}