#include <stdio.h>
#include <windows.h>
#include <conio.h>
int main(){
	HANDLE hPipe;
	char MessageIn[256]="", MessageOut[256]="",MessageIn1[256]="";
	DWORD BytesRead, BytesWrite;
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE; 
	InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd,TRUE,NULL,FALSE); 
	sa.lpSecurityDescriptor = &sd;

	hPipe = CreateNamedPipe(
				"\\\\.\\pipe\\MyPipe",
				PIPE_ACCESS_DUPLEX, 
				PIPE_TYPE_MESSAGE| PIPE_WAIT, 
				1, 
				0, 
				0, 
				INFINITE, 
				&sa); 

   if (hPipe == INVALID_HANDLE_VALUE){
		printf("CreatePipe failed: error code %d\n", (int)GetLastError());
		return 0;
	}
	
	if ((ConnectNamedPipe(hPipe,NULL)) == 0){ 
		printf("Client could not connect\n");
		return 0;
	}
	else
		printf("Client connected!\n");
	while(1){

	
		if(kbhit()){
			printf("ServerToClient:  ");
			scanf("%s",MessageOut);
		}
		
		if (MessageOut != NULL) WriteFile(
			hPipe,
			MessageOut,
			sizeof(MessageOut),
			&BytesWrite,
			NULL);
			
		ReadFile(
			hPipe,
			MessageIn,
			sizeof(MessageIn),
			&BytesRead, 
			NULL); 

		if (strcmp(MessageIn1, MessageIn) != 0){
				printf("ClientToServer:  %s\n",MessageIn);
				memset(MessageIn1,0,256);
				strcpy(MessageIn1,MessageIn);
			}
	}
	CloseHandle(hPipe);
	system("PAUSE");
	return 0;
}