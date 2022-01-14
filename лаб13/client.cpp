#include <windows.h>
#include <stdio.h>
#include <conio.h>
int main(){
	char c; 
	HANDLE hPipe;
	char MessageIn[256]="", MessageOut[256]="", machineName[80], pipeName[80],MessageIn1[256]="";
	DWORD BytesWrite, BytesRead;

	printf("Enter name: ");
	scanf("%s",&machineName);
	sprintf(pipeName,"\\\\%s\\pipe\\MyPipe",machineName);
	hPipe = CreateFile(
				pipeName,
				GENERIC_READ|GENERIC_WRITE,
				FILE_SHARE_READ|FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING, 
				0,
				NULL);
				
   if (hPipe == INVALID_HANDLE_VALUE){
		printf("CreatePipe failed: error code %d\n", (int)GetLastError());
		return 0;
	}
	while(1){
		if (!ReadFile(
			hPipe,
			MessageIn,
			sizeof(MessageIn), 
			&BytesRead, 
			NULL)){ 
				printf("\nDisconnected.\nPress any key.");
				break;
		}
		else {
			if (strcmp(MessageIn1, MessageIn) != 0){
				printf("FromServer:  %s\n", MessageIn);
				memset(MessageIn1,0,256);
				strcpy(MessageIn1,MessageIn);
			}
		}

		if (kbhit()){
			printf("ToServer:  ");
			scanf("%s",MessageOut);
		}
		WriteFile(
			hPipe,
			MessageOut,
			sizeof(MessageOut),
			&BytesWrite,
			NULL);
	}
	scanf("%c",&c);
	CloseHandle(hPipe);
	system("PAUSE");
	return 0;
}