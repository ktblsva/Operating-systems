#include <stdio.h>
#include <windows.h>
#include <conio.h>
int main(){
	HANDLE hPipe;
	char MessageIn[256]="", MessageOut[256]="",MessageIn1[256]="";
	DWORD BytesRead, BytesWrite;
	
	///////////////
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE; //äåñêðèïòîð êàíàëà íå íàñëåäóåìûé
	InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd,TRUE,NULL,FALSE); //ðàçðåøèëè äîñòóï âñåì ïîëüçîâàòåëÿì
	sa.lpSecurityDescriptor = &sd;
	///////////////
	
	hPipe = CreateNamedPipe(
				"\\\\.\\pipe\\MyPipe",
				PIPE_ACCESS_DUPLEX, //÷èòàåì èç êàíàëà è ïèøåì â êàíàë
				PIPE_TYPE_MESSAGE| PIPE_WAIT, //ñèíõðîííàÿ ïåðåäà÷à ñîîáùåíèé
				1, //ìàêñèìàëüíîå êîëè÷åñòâî ýêçåìïëÿðîâ êàíàëà
				0, //ðàçìåð âûõîäíîãî áóôåðà ïî óìîë÷àíèþ
				0, //ðàçìåð âõîäíîãî áóôåðà ïî óìîë÷àíèþ
				INFINITE, //îæèäàíèå ñâÿçè 500 ìñ
				&sa); //çàùèòà ïî óìîë÷àíè (äîñòóï ïî èìåíè êîìïà)

   if (hPipe == INVALID_HANDLE_VALUE){
		printf("CreatePipe failed: error code %d\n", (int)GetLastError());
		return 0;
	}
	
	if ((ConnectNamedPipe(hPipe,NULL)) == 0){ //ïðîâåðêà íà ñâÿçü
		printf("Client could not connect\n");
		return 0;
	}
	else
		printf("		SOMEONE IS CONNECT!\n");
	while(1){

		//îòïðàâêà ñîîáùåíèÿ
		if(kbhit()){
			printf("Your message:  ");
			scanf("%s",MessageOut);
		}
		
		if (MessageOut != NULL) WriteFile(
			hPipe,
			MessageOut,
			sizeof(MessageOut),
			&BytesWrite,
			NULL);
			


		//÷òåíèå ñîîáùåíèÿ
		ReadFile(
			hPipe,
			MessageIn,
			sizeof(MessageIn), // ÷èñëî ÷èòàåìûõ äàííûõ
			&BytesRead, //÷èñëî ïðî÷èòàííûé áàéò
			NULL); //ïåðåäà÷à äàííûõ ñèíõðîííàÿ

		if (strcmp(MessageIn1, MessageIn) != 0){
				printf("Someone's' message:  %s\n",MessageIn);
				memset(MessageIn1,0,256);
				strcpy(MessageIn1,MessageIn);
			}

	}
	
	CloseHandle(hPipe);
	system("PAUSE");
	return 0;
}