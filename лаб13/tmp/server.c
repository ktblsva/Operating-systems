#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")

#define MY_PORT 1952

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
            clientInfo . clientAddr = clientAddr ;
            clientInfo . hClientSocket = hClientSocket ;
            struct hostent *hst;
            int bytes_recv;
            hst=gethostbyaddr((char*)&clientAddr.sin_addr.s_addr, 4, AF_INET);
            printf("%s [%s] new connect!\n", (hst)?hst->h_name:"", inet_ntoa(clientAddr.sin_addr));
            hClientThread = CreateThread( NULL, 0,
                ( LPTHREAD_START_ROUTINE ) ClientThread,
                ( LPVOID ) &clientInfo, 0, &dwThreadId ) ;
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
    closesocket(mysocket);
    WSACleanup();
    return 0;
}

DWORD WINAPI ClientThread(LPVOID lpData)
{
    struct CLIENT_INFO *pCI = (struct CLIENT_INFO*) lpData;
    //SYSTEMTIME stSystemTime;
	//GetSystemTime(&stSystemTime);
    char buff[1024];
    int bytes_recv;
    char date[10] = {'\0'};
    char filename[CHAR_MAX];
    struct hostent *hst;
    hst=gethostbyaddr((char*)&pCI->clientAddr.sin_addr.s_addr, 4, AF_INET);
    //sprintf(date, "[%d-%d-%d %d.%d.%d]", stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay, stSystemTime.wHour, stSystemTime.wMinute,
	//		 												stSystemTime.wSecond);
    sprintf(filename, "%s %s.txt", date, (hst)?hst->h_name:"" );
    // FILE* fp = fopen(filename, "a");
    send(pCI->hClientSocket, "Hello a new client!\n", sizeof("Hello a new client\n"), 0);
    while((bytes_recv=recv(pCI->hClientSocket, &buff[0], sizeof(buff), 0)) && bytes_recv!=SOCKET_ERROR) 
    {
        buff[bytes_recv] = 0;
        //fprintf(fp, "%s", buff);
        printf("ClientToServer: %s", buff);
        printf("From server to %s: ",inet_ntoa(pCI->clientAddr.sin_addr));
        fgets(&buff[0], sizeof(buff)-1, stdin);
        if(!strcmp(&buff[0], "quit\n"))
        {
             printf("Exit...");
             //fclose(fp);
             closesocket(mysocket);
             WSACleanup();
             return TRUE;
        }
        send(pCI->hClientSocket, &buff[0], bytes_recv, 0);
    }
    printf("Client %s was disconnected\n", inet_ntoa(pCI->clientAddr.sin_addr));
    //fclose(fp);
    closesocket(pCI->hClientSocket);
    return TRUE;
}