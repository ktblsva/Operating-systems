#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <string.h>
#include <time.h>
#include <process.h>
#include <stdio.h>
#include <imagehlp.h>
#include <locale.h>
#include <tchar.h>
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "imagehlp.lib")
#define LOGPATH "D:\\log.log"
#define DATAPATH "D:\\data.txt"
#define CONFIGPATH "D:\\config111.txt"
#pragma comment(lib, "Ws2_32.lib")
#define PORT 1952
//#define SERVERADDR "192.168.1.152"
//#define SERVERADDR "192.168.1.253"
#define SERVERADDR "127.0.0.1"

//cmd.exe /k "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars32.bat" `& powershell 
SERVICE_STATUS          wserv_testStatus; 
SERVICE_STATUS_HANDLE   wserv_testStatusHandle; 
char pathToFile[CHAR_MAX];
int delayForUpdate = 1;
using namespace std;

void writeToLogfile(char* error)
{
	FILE* flog = fopen(LOGPATH, "a");
	SYSTEMTIME stSystemTime;
	GetSystemTime(&stSystemTime);
	fprintf(flog,"[%d-%d-%d %d:%d:%d] %s\n",stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay,stSystemTime.wHour, stSystemTime.wMinute,
			 												stSystemTime.wSecond, error);
	fclose(flog);
}

void writeToLogfile(int error)
{
	FILE* flog = fopen(LOGPATH, "a");
	SYSTEMTIME stSystemTime;
	GetSystemTime(&stSystemTime);
	fprintf(flog,"[%d-%d-%d %d:%d:%d] Delay for update is %d\n",stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay,stSystemTime.wHour, stSystemTime.wMinute,
			 												stSystemTime.wSecond, error);
	fclose(flog);
}


void Export(IMAGE_NT_HEADERS *pNtHdr, LPVOID pSrcFile){
	char buff[CHAR_MAX] = {'\0'};
	IMAGE_EXPORT_DIRECTORY* ExpTable;
	char *pName, *sName, **pNames;
	DWORD nNames;
	DWORD RVAExpDir, VAExpAddress;
	FILE *fp = fopen(DATAPATH, "a");
	int i;
	RVAExpDir = pNtHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	VAExpAddress = (DWORD)ImageRvaToVa(pNtHdr, pSrcFile, RVAExpDir, NULL);
	ExpTable=(IMAGE_EXPORT_DIRECTORY*)VAExpAddress;
	if(ExpTable == 0)
	{
		writeToLogfile("There is no exported data.");
		return;
	}
	sName=(char*)ImageRvaToVa(pNtHdr,pSrcFile,ExpTable->Name,NULL);
	fprintf(fp, "Name of PEF:  %s\n",sName);
	pNames=(char**)ImageRvaToVa(pNtHdr,pSrcFile,ExpTable->AddressOfNames,NULL);
	nNames=ExpTable->NumberOfNames;
	fprintf(fp, "Exported data: \n");
	for(i = 0; i < nNames; i++){
		if (wserv_testStatus.dwCurrentState == SERVICE_STOPPED)
		{
			fclose(fp);
			return;
		}
		while (wserv_testStatus.dwCurrentState == SERVICE_PAUSED) 
		{ 
			if (wserv_testStatus.dwCurrentState == SERVICE_STOPPED)
			{
				fclose(fp);
				return;
			}
		}
		pName = (char*)ImageRvaToVa(pNtHdr,pSrcFile,(DWORD)*pNames,NULL);
		fprintf(fp, "	%s\n",pName);
		*pNames++;
	}
	fprintf(fp, "\n");
	fclose(fp);
}

void Import(IMAGE_NT_HEADERS *pNtHdr, LPVOID pSrcFile){
	char *pName = nullptr, *sName, **pNames;
	DWORD nNames;
	DWORD RVAExpDir, VAExpAddress;
	IMAGE_IMPORT_DESCRIPTOR* ImportTable;
	
	RVAExpDir = pNtHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress; 
	VAExpAddress = (DWORD)ImageRvaToVa(pNtHdr,pSrcFile,RVAExpDir,NULL);
	ImportTable=(IMAGE_IMPORT_DESCRIPTOR*)VAExpAddress;
	if(ImportTable == NULL)
	{
		writeToLogfile("There is no imported data.");
		return;
	}
	FILE *fp = fopen(DATAPATH, "a");
	fprintf(fp, "Imported data: \n");
	while(ImportTable->Name != NULL){
		pNames=(char**)ImageRvaToVa(pNtHdr,pSrcFile,ImportTable->FirstThunk,NULL);
		sName=(char*)ImageRvaToVa(pNtHdr,pSrcFile,ImportTable->Name,NULL); 
		char buff[CHAR_MAX] = {'\0'};
		fprintf(fp, "Name of PEF:  %s\n",sName);
		while(pName != 0)
		{
			if (wserv_testStatus.dwCurrentState == SERVICE_STOPPED)
			{
				fclose(fp);
				return;
			}
			while (wserv_testStatus.dwCurrentState == SERVICE_PAUSED) 
			{ 
				if (wserv_testStatus.dwCurrentState == SERVICE_STOPPED)
				{
					fclose(fp);
					return;
				}
			}
			pName = (char*)ImageRvaToVa(pNtHdr,pSrcFile,(DWORD)*pNames+2,NULL);
			if(pName != 0){
				fprintf(fp, "     %s\n",pName);
  		 	}
			*pNames++;
		}
		*pName++;
		ImportTable++;
	}
	fprintf(fp, "\n\n");
	fclose(fp);
}

void readConfigFile()
{
	FILE* fp = fopen(CONFIGPATH, "r");
	char delayForUpdate_[CHAR_MAX];
	char* r;
    char buff[CHAR_MAX];
	if (fp == NULL) { 
		writeToLogfile("Error: can't open configuration file."); 
		return; 
	}
    r = fgets(pathToFile, sizeof(pathToFile), fp);
	if (r == NULL)
	{
		if (feof(fp) != 0)
		{
			writeToLogfile(&pathToFile[0]);
			writeToLogfile("Error: wrong configuration file!");
			return;
		}
		else
		{
			writeToLogfile(&pathToFile[0]);
			writeToLogfile("Error: can't read configuration file!");
			return;
		}
	}
	pathToFile[strlen(pathToFile) - 1] = '\0';
	writeToLogfile(&pathToFile[0]);
	r = fgets(delayForUpdate_, sizeof(delayForUpdate_), fp);
	if (r == NULL)
	{
		if (feof(fp) != 0)
		{
			writeToLogfile("Error: wrong configuration file! Set default value.");
		}
		else
		{
			writeToLogfile("Error: can't read configuration file!");
			return;
		}
	}
	const char* tmp = delayForUpdate_;
	delayForUpdate = atoi(tmp);
	writeToLogfile(delayForUpdate);
	fclose(fp);
	writeToLogfile("Exit from readconfigfile!");
}

int checkPEFile()
{
	HANDLE hFileMap, hFile;
	LPVOID pSrcFile;
	IMAGE_DOS_HEADER *pDosHeader;
	IMAGE_NT_HEADERS *pNtHdr;
	IMAGE_SECTION_HEADER *pFirstSectionHeader, *pSectionHeader;
	readConfigFile();
	writeToLogfile("After exit from readconfigfile!");
	char* log;
	sprintf(log, "Trying to open %s", pathToFile);
	writeToLogfile(log);
	writeToLogfile(delayForUpdate);

	hFile = CreateFileA(pathToFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL); 
	if (hFile == INVALID_HANDLE_VALUE)
	{
		writeToLogfile("Couldn't open file!");
		return -1;
	}
		
	hFileMap = CreateFileMapping (hFile, NULL, PAGE_READONLY, 0, 0, NULL); 
	if(hFileMap == NULL)
	{
		writeToLogfile("Could not create mapping file.");
		return -1;
	}
		
	pSrcFile = (PBYTE) MapViewOfFile(hFileMap,FILE_MAP_READ,0,0,0); 
	if(pSrcFile == NULL)
	{
		writeToLogfile("Could not map file.");
		return -1;
	}
 	pDosHeader = (IMAGE_DOS_HEADER *)pSrcFile; 
	pNtHdr = (IMAGE_NT_HEADERS *)((DWORD)pDosHeader + pDosHeader->e_lfanew); 
	Export(pNtHdr,pSrcFile);
	Import(pNtHdr,pSrcFile);
	return 0;
}

int sendFile(char* filename, SOCKET my_sock)
{
    char buff[1024];
    printf("Sending data...\n");
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) { 
		printf("Error: can't open configuration file.\n"); 
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
	return 0;
}

int receiveFile(SOCKET sock)
{
    FILE* fp = fopen(CONFIGPATH, "w");
    int length = 0;
    char buff[1024];
    int bytes_recv;
    memset(buff, 0, sizeof(buff));
    //printf("file opened\n");
    while ((length = recv(sock, buff, sizeof(buff), 0)) > 0) 
    { 
        //printf("%s\n", buff);
		writeToLogfile(buff);
		int breakAfter = 0;
		char* p;
        if((p = strstr(buff, "<end of file>\n")) != NULL)
        {
            breakAfter = p - buff;
			memcpy(buff,buff,breakAfter);
			writeToLogfile(buff);
			length = breakAfter;
            breakAfter=1;
        } 
        else if ((p = strstr(buff, "Error\n")) != NULL)
        {
			fclose(fp);
            return -1;
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
    //printf("Recv error %d\n", WSAGetLastError());
    fclose(fp);
    printf("Receive File: %s from client successful!\n", CONFIGPATH); 
	return 0;
}

int clientApp()
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
    sprintf(toSend, "%s connected\n", localName);
    while((nsize=recv(my_sock, &buff[0], sizeof(buff)-1, 0)) != SOCKET_ERROR)
    {
        buff[nsize] = 0;
        printf("Server To Client: %s", buff);
        if(!strcmp(buff, "Connected!\n"))
        {
            send(my_sock, toSend, sizeof(toSend), 0);
            int res = receiveFile(my_sock);
            if (res == -1)
            {
				writeToLogfile("Error from server!");
                break;
            }
			writeToLogfile("After receiving config file");
			res = 0;
			res = checkPEFile();
			if (res == -1)
			{
				writeToLogfile("Error when reading PE file!");
                break;
			}
			writeToLogfile("Exit from PE");
            //printf("_____________\n");
        } else if(!strcmp(buff, "Waiting for data...\n"))
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


VOID __stdcall CtrlHandler (DWORD Opcode) 
{     
	DWORD status;  
    switch(Opcode)     
	{         
	case SERVICE_CONTROL_PAUSE: 
            wserv_testStatus.dwCurrentState = SERVICE_PAUSED;
			break; 
         case SERVICE_CONTROL_CONTINUE: 
            wserv_testStatus.dwCurrentState = SERVICE_RUNNING; 
            break;          
		 case SERVICE_CONTROL_STOP: 
            wserv_testStatus.dwWin32ExitCode = 0; 
            wserv_testStatus.dwCurrentState  = SERVICE_STOPPED; 
            wserv_testStatus.dwCheckPoint    = 0; 
            wserv_testStatus.dwWaitHint      = 0;  
            if (!SetServiceStatus (wserv_testStatusHandle, 
                &wserv_testStatus))            
			{ 
                status = GetLastError(); 
            }  
            return;          
		 default: 
         	 break;

	}      

    if (!SetServiceStatus (wserv_testStatusHandle,  &wserv_testStatus))     
	{ 
        status = GetLastError(); 
	}     
	return; 
}  

void __stdcall wserv_testStart (DWORD argc, LPTSTR *argv) 
{     
	DWORD status; 

    wserv_testStatus.dwServiceType        = SERVICE_WIN32; 
    wserv_testStatus.dwCurrentState       = SERVICE_START_PENDING; 
    wserv_testStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | 
        SERVICE_ACCEPT_PAUSE_CONTINUE; 
    wserv_testStatus.dwWin32ExitCode      = 0; 
    wserv_testStatus.dwServiceSpecificExitCode = 0; 
    wserv_testStatus.dwCheckPoint         = 0; 
    wserv_testStatus.dwWaitHint           = 0;  
    wserv_testStatusHandle = RegisterServiceCtrlHandler( 
        TEXT("wserv_test"),         CtrlHandler);  
   
	if (wserv_testStatusHandle == (SERVICE_STATUS_HANDLE)0)     
		return;     
	  
    wserv_testStatus.dwCurrentState       = SERVICE_RUNNING; 
    wserv_testStatus.dwCheckPoint         = 0; 
    wserv_testStatus.dwWaitHint           = 0;  
    if (!SetServiceStatus (wserv_testStatusHandle, &wserv_testStatus))     
	{ 
        status = GetLastError(); 
	}  

	FILE* fp;
	SYSTEMTIME stSystemTime;

	while (wserv_testStatus.dwCurrentState!=SERVICE_STOPPED)
	{
		if (wserv_testStatus.dwCurrentState!=SERVICE_PAUSED){
			GetSystemTime(&stSystemTime);
			fp = fopen(DATAPATH, "a");
			fprintf(fp,"[%d-%d-%d %d:%d:%d]\n",stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay,stSystemTime.wHour, stSystemTime.wMinute,
			 												stSystemTime.wSecond);
			fclose(fp);
			int res = clientApp();
			if(res != 0)
			{
				checkPEFile();
			}
		}
		Sleep(60000 * delayForUpdate);
	}

	return; 
} 

void main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Russian");
	SERVICE_TABLE_ENTRY   DispatchTable[] =     
	{ 
        { TEXT("wserv_test"), wserv_testStart      }
		,{ NULL,              NULL                }     
	};
	if (argc>1 && (!stricmp(argv[1],"stop") || !stricmp(argv[1],"delete")))
	{
		SC_HANDLE scm=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
		if (!scm) 
		{
			cout<<"Can't open SCM\n";
			exit(1);
		}
		SC_HANDLE svc=OpenService(scm,"wserv_test",SERVICE_STOP | SERVICE_QUERY_STATUS );
		if (!svc)
		{
			cout<<"Can't open service\n";
			exit(2);
		}
		DWORD dwBytesNeeded{0};
		SERVICE_STATUS_PROCESS ssp {0};
		QueryServiceStatusEx( svc, SC_STATUS_PROCESS_INFO, (LPBYTE)( &ssp), sizeof(SERVICE_STATUS_PROCESS), &dwBytesNeeded );
		if (ssp.dwCurrentState != SERVICE_STOPPED)
		{
			if (ControlService(svc, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)&ssp))
			{			
				if (!(ssp.dwCurrentState == SERVICE_STOP_PENDING || ssp.dwCurrentState == SERVICE_STOPPED))
				{
					cout << "Can't stop service\n";
					exit(3);
				}
			}
			cout<<"Service stopped\n";
			CloseServiceHandle(svc);
			CloseServiceHandle(scm);

			if (stricmp(argv[1],"delete"))
			{
				exit(0);
			}
		}
		
	}
	if (argc>1 && !stricmp(argv[1],"pause"))
	{
		SC_HANDLE scm=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
		if (!scm) 
		{
			cout<<"Can't open SCM\n";
			exit(1);
		}
		SC_HANDLE svc=OpenService(scm,"wserv_test", SERVICE_ALL_ACCESS);
		if (!svc)
		{
			cout<<"Can't open service\n";
			exit(2);
		}
	
		SERVICE_STATUS_PROCESS ssp {0};
		if (ControlService(svc, SERVICE_CONTROL_PAUSE, (LPSERVICE_STATUS)&ssp))
		{			
			if (!(ssp.dwCurrentState == SERVICE_PAUSE_PENDING || ssp.dwCurrentState == SERVICE_PAUSED))
			{
				cout << "Can't pause service\n";
				exit(3);
			}
		}
		cout<<"Service paused\n";
		CloseServiceHandle(svc);
		CloseServiceHandle(scm);
		exit(0);
	}
	if (argc>1 && !stricmp(argv[1],"continue"))
	{
		SC_HANDLE scm=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
		if (!scm) 
		{
			cout<<"Can't open SCM\n";
			exit(1);
		}
		SC_HANDLE svc=OpenService(scm,"wserv_test", SERVICE_ALL_ACCESS);
		if (!svc)
		{
			cout<<"Can't open service\n";
			exit(2);
		}
	
		SERVICE_STATUS_PROCESS ssp {0};
		if (ControlService(svc, SERVICE_CONTROL_CONTINUE, (LPSERVICE_STATUS)&ssp))
		{			
			if (!(ssp.dwCurrentState == SERVICE_CONTINUE_PENDING || ssp.dwCurrentState == SERVICE_RUNNING))
			{
				cout << "Can't continue service\n";
				exit(3);
			}
		}
		cout<<"Service running\n";
		CloseServiceHandle(svc);
		CloseServiceHandle(scm);
		exit(0);
	}
	if (argc>1 && !stricmp(argv[1],"delete"))
	{
		SC_HANDLE scm=OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE);
		if (!scm) 
		{
			cout<<"Can't open SCM\n";
			exit(1);
		}
		SC_HANDLE svc=OpenService(scm,"wserv_test",DELETE);
		if (!svc)
		{
			cout<<"Can't open service\n";
			exit(2);
		}
		if (!DeleteService(svc))
		{
			cout<<"Can't delete service\n";
			exit(3);
		}
		cout<<"Service deleted\n";
		CloseServiceHandle(svc);
		CloseServiceHandle(scm);
		exit(0);
	}
	if (argc>1 && !stricmp(argv[1],"start"))
	{
		SC_HANDLE scm=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
		if (!scm) 
		{
			cout<<"Can't open SCM\n";
			exit(1);
		}
		SC_HANDLE svc=OpenService(scm,"wserv_test",SERVICE_ALL_ACCESS);
		if (!svc)
		{
			cout<<"Can't open service\n";
			exit(2);
		}
		if (!StartService(svc, 0, NULL))
		{
			cout<<"Can't start service\n";
			exit(3);
		}
		cout<<"Service started\n";
		CloseServiceHandle(svc);
		CloseServiceHandle(scm);

		exit(0);
		
	}
	if (argc>1 && !stricmp(argv[1],"setup"))
	{
		char pname[1024];
		pname[0]='"';
		GetModuleFileName(NULL, pname+1, 1023);
		strcat(pname,"\"");
		SC_HANDLE scm=OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE),svc;
		if (!scm) 
		{
			cout<<"Can't open SCM\n";
			exit(1);
		}
		if (!(svc=CreateService(scm,"wserv_test","wserv_test",SERVICE_ALL_ACCESS,
			SERVICE_WIN32_OWN_PROCESS,SERVICE_DEMAND_START,
			SERVICE_ERROR_NORMAL,pname,NULL,NULL,NULL,NULL,NULL)))
		{
			cout<<"Registration error!\n";
			exit(2);
		}
		cout<<"Successfully registered "<<pname<<"\n";
		CloseServiceHandle(svc);
		CloseServiceHandle(scm);
		exit(0);
	}

	if (!StartServiceCtrlDispatcher(DispatchTable))     
	{ 
        
	} 
}