#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <string.h>
#define LOGPATH "D:\\log.log"
#define DATAPATH "D:\\data.txt"
#define CONFIGPATH "D:\\config.txt"
#pragma comment(lib, "Ws2_32.lib")

#define PORT 1952
//#define SERVERADDR "192.168.1.152"
#define SERVERADDR "127.0.0.1"

void sendFile(char* filename, SOCKET my_sock)
{
    char buff[1024];
    printf("Sending data...\n");
    FILE* fp = fopen(filename, "r");
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
    printf("File: transfer successful!\n");
}

int main(int argc, char* argv[])
{
    char buff[1024];
    struct sockaddr_in dest_addr;
    SOCKET my_sock;
    struct hostent *hst;
    int nsize;
    if(WSAStartup(0x202, (WSADATA *)&buff[0]))
    {
        printf("WSAStart error %d\n", WSAGetLastError());
        return -1;
    }
    my_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(my_sock < 0)
    {
        printf("Socket() error %d\n", WSAGetLastError());
        return -1;
    }
    dest_addr.sin_family=AF_INET;
    dest_addr.sin_port=htons(PORT);
    if(inet_addr(SERVERADDR)!=INADDR_NONE)
        dest_addr.sin_addr.s_addr=inet_addr(SERVERADDR);
    else if(gethostbyname(SERVERADDR))
        ((unsigned long*)&dest_addr.sin_addr)[0]=((unsigned long**)hst->h_addr_list)[0][0];
        else {
            printf("Invalid address %s\n", SERVERADDR);
            closesocket(my_sock);
            WSACleanup();
            return -1;
        }
    if(connect(my_sock, (struct sockaddr*)&dest_addr, sizeof(dest_addr)))
    {
        printf("Connect error %d\n", WSAGetLastError());
        return -1;
    }
    printf("Connection with %s was estabilished\n\n Type quit for quit\n\n", SERVERADDR);
    char localName[100], toSend[100];
    if(gethostname(localName, sizeof(localName)) == SOCKET_ERROR)
    {
         printf("Error: wrong local name %d\n", WSAGetLastError());
    }
    sprintf(toSend, "%s\n", localName);
    while((nsize=recv(my_sock, &buff[0], sizeof(buff)-1, 0)) != SOCKET_ERROR)
    {
        buff[nsize] = 0;
        printf("ServerToClient: %s", buff);
        if(!strcmp(buff, "Connected!\n"))
        {
            send(my_sock, toSend, sizeof(toSend), 0);
        } else
        {
            sendFile(DATAPATH, my_sock);
            send(my_sock, "<end of file>\n", sizeof("<end of file>\n"), 0);
			Sleep(500);
            sendFile(LOGPATH, my_sock);
            send(my_sock, "<end of file>\n", sizeof("<end of file>\n"), 0);
            printf("Exit...");
            closesocket(my_sock);
            WSACleanup();
            return 0;
        }
    }
    printf("Recv error %d\n", WSAGetLastError());
    closesocket(my_sock);
    WSACleanup();
    return -1;
}