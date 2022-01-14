#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")

#define MY_PORT 1952
#define CONFIG_FILE "config.txt"

struct CLIENT_INFO
{
    SOCKET hClientSocket ;
    struct sockaddr_in clientAddr ;
};

DWORD WINAPI ClientThread( LPVOID lpData );
SOCKET mysocket;

int main(int argc, char* argv[])
{
    char buff[1024];
    struct sockaddr_in local_addr;
    if(WSAStartup(0x0202, (WSADATA *) &buff[0]))
    {
        printf("Error WSAStartup %d\n", WSAGetLastError());
        return -1;
    }

    if((mysocket=socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        printf("Error socket %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }
    local_addr.sin_family=AF_INET;
    local_addr.sin_port=htons(MY_PORT);
    local_addr.sin_addr.s_addr=0;
    if(bind(mysocket, (struct sockaddr *) &local_addr, sizeof(local_addr)))
    {
        printf("Error bind %d\n", WSAGetLastError());
        closesocket(mysocket);
        WSACleanup();
        return -1;
    }
    if(listen(mysocket, 0x100))
    {
        printf("Error listen %d\n", WSAGetLastError());
        closesocket(mysocket);
        WSACleanup();
        return -1;
    }
    printf("Waiting for calls\n");
    while (1)
    {
        SOCKET hClientSocket ;
        struct sockaddr_in clientAddr ;
        int nSize = sizeof(clientAddr) ;
        hClientSocket = accept(mysocket, ( struct sockaddr *) &clientAddr, &nSize);
        if (hClientSocket != INVALID_SOCKET)
        {
            HANDLE hClientThread ;
            struct CLIENT_INFO clientInfo ;
            DWORD dwThreadId ;
            clientInfo.clientAddr = clientAddr ;
            clientInfo.hClientSocket = hClientSocket ;
            struct hostent *hst;
            int bytes_recv;
            hst=gethostbyaddr((char*)&clientAddr.sin_addr.s_addr, 4, AF_INET);
            printf("%s [%s] new connect!\n", (hst)?hst->h_name:"", inet_ntoa(clientAddr.sin_addr));
            hClientThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ClientThread, (LPVOID) &clientInfo, 0, &dwThreadId);
			if ( hClientThread == NULL )
            {
                printf("can't create thread.\n");
            }
            else
            {
                CloseHandle( hClientThread ) ;
            }
        }
        else  printf("Error accept %d\n", WSAGetLastError());;
    }
    printf("By myself!\n");
    closesocket(mysocket);
    WSACleanup();
    return 0;
}

void receiveFile(struct CLIENT_INFO *pCI, char* suffix)
{
    SYSTEMTIME stSystemTime;
	GetSystemTime(&stSystemTime);
    char date[255];
    char filename[CHAR_MAX];
    struct hostent *hst;
    hst=gethostbyaddr((char*)&pCI->clientAddr.sin_addr.s_addr, 4, AF_INET);
    sprintf(date, "[%d-%d-%d %d.%d.%d]\0", stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay, stSystemTime.wHour, stSystemTime.wMinute,
			 												stSystemTime.wSecond);
    sprintf(filename, "%s %s %s.txt", date, (hst)?hst->h_name:"", suffix);
    FILE* fp = fopen(filename, "a");
    int length = 0;
    char buff[1024];
    int bytes_recv;
    memset(buff, 0, sizeof(buff));
    while ((length = recv(pCI->hClientSocket, buff, sizeof(buff), 0)) > 0) 
    { 
        //printf("%s\n", buff);
		int breakAfter = 0;
		char* p;
        if((p = strstr(buff, "<end of file>\n")) != NULL)
        {
            breakAfter = p - buff;
			memcpy(buff,buff,breakAfter);
			length = breakAfter;
            breakAfter=1;
        } 
        if (fwrite(buff, sizeof(char), length, fp) < length) 
        { 
            printf("File: Write Failed\n"); 
            break; 
        } 
		if (breakAfter)
		{
			break;
		}
        // fprintf(fp, "%s", buff);
        memset(buff, 0, sizeof(buff)); 
    } 
    fclose(fp);
    printf("Receive File: %s from client successful!\n", filename); 
}

int sendFile(char* filename, SOCKET my_sock)
{
    char buff[1024];
    printf("Sending data...\n");
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) { 
		printf("Error: can't open configuration file.\n"); 
        send(my_sock, "Error\n", sizeof("Error\n"), 0);
		return -1; 
	}
    memset(buff, 0, sizeof(buff)); 
    char* r; 
    while ((r = fgets(buff, sizeof(buff), fp)) != 0)
    { 
        if (send(my_sock, buff, strlen(buff), 0) < 0) 
        { 
            printf("Send File: failed\n"); 
            break; 
        } 
            memset(buff, 0, sizeof(buff)); 
    } 
    fclose(fp); 
    printf("File: transfer %s successful!\n", filename);
}

DWORD WINAPI ClientThread(LPVOID lpData)
{
    struct CLIENT_INFO *pCI = (struct CLIENT_INFO*) lpData;
    char buff[1024];
    int bytes_recv;
    send(pCI->hClientSocket, "Connected!\n", sizeof("Connected!\n"), 0);
    
    while((bytes_recv=recv(pCI->hClientSocket, &buff[0], sizeof(buff), 0)) && bytes_recv!=SOCKET_ERROR) 
    {
        buff[bytes_recv] = 0;
        printf("Client %s to Server: %s", inet_ntoa(pCI->clientAddr.sin_addr), buff);
        //printf("Enter name of config file: ");
        //fgets(&buff[0], sizeof(buff)-1, stdin);
        //buff[strlen(buff) - 1] = '\0';
        int res = sendFile(CONFIG_FILE, pCI->hClientSocket);
        if (res == -1)
        {
            break;
        }
        memset(buff, 0, sizeof(buff));
        send(pCI->hClientSocket, "\n<end of file>\n", sizeof("\n<end of file>\n"), 0);
        Sleep(500);
        sprintf(buff, "Waiting for data...\n");
        send(pCI->hClientSocket, &buff[0], bytes_recv, 0);
        receiveFile(pCI, "data");
        receiveFile(pCI, "log");
    }
    printf("Client %s was disconnected\n", inet_ntoa(pCI->clientAddr.sin_addr));
    closesocket(pCI->hClientSocket);
    return 0;
}