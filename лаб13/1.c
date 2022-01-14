#include <windows.h>
#include <stdio.h>
#include <conio.h>
int main() {
	char c; 
	HANDLE hPipe;
	char MessageIn[256] = "", MessageOut[256] = "", machineName[80], pipeName[80], MessageIn1[256] = "";
	DWORD BytesWrite, BytesRead;
	int delayedQuit = 0;
	//DESKTOP-FQ5V6LA.sib.mts.ru
	printf("Name of server: ");
	scanf("%s", &machineName);
	sprintf(pipeName, "\\\\%s\\pipe\\MyPipe", machineName);
	hPipe = CreateFile(
		pipeName,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING, 
		0,
		NULL);

	if (hPipe == INVALID_HANDLE_VALUE) {
		printf("CreatePipe failed: error code %d\n", (int)GetLastError());
		return 0;
	}
	printf("%s looking forward to your messages.\n \t (Type \"Quit\" to leave chat)\n", machineName);
	while (1) {
		if (!ReadFile(
			hPipe,
			MessageIn,
			sizeof(MessageIn), 
			&BytesRead, 
			NULL)) { 
			printf("%s was close.\nPress any key to out.", machineName);
			break;
		}
		else {
			if (strcmp(MessageIn1, MessageIn) != 0) {
				printf("%s message:  %s\n", machineName, MessageIn);
				memset(MessageIn1, 0, 256);
				strcpy(MessageIn1, MessageIn);
			}


		}

		if (kbhit()) {
			gets(MessageOut);
			if (strcmp(MessageOut,"Quit") == 0)
			{
				sprintf(MessageOut,"<Quit>");
				delayedQuit = 1;
			}
		}

		WriteFile(
			hPipe,
			MessageOut,
			sizeof(MessageOut),
			&BytesWrite,
			NULL);
		
		if (delayedQuit)
		{
			break;
		}

	}

	//scanf("%c", &c);
	CloseHandle(hPipe);
	system("PAUSE");
	return 0;
}