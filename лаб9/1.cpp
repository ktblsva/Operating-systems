#include <windows.h>
#include <thread>
#include <stdio.h>
#include <chrono>
#include <process.h>
#define N 20
#define MAX 100
//cl /MT /D "_X86_" /EHsc 1.cpp
//cmd.exe /k "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars32.bat" `& powershell 
using namespace std;

CRITICAL_SECTION cs;
int Count = 0;
bool readyFlags[N];
int turn;

/*
int turn[N - 1];
void EnterCriticalRegion(int tID)
{
	for (int level = 0; level < N - 1; ++level)
	{
		readyFlags[tID] = level + 1;
		turn[level] = tID;
		
		while(true)
		{
			int found = false;
			for (int thread = 0; !found && thread != N; ++thread)
			{
				if (thread == tID) {continue;}
				found = readyFlags[thread] > level;
			}
			if (!found) break;
			if (turn[level] != tID) break;
		}
	}
}


void LeaveCriticalRegion(int tID)
{
	readyFlags[tID] = false;
}
*/

void EnterCriticalRegion(int tID)
{
	readyFlags[tID] = true;
	int i = 0;
	for (i = 0; i < N; i++)
	{
		if(readyFlags[i] && i != tID)
		{
			turn = i;
			break;
		}
	}
	while(turn == i && readyFlags[i]);
	//printf("%d enter\n", tID);
}

void LeaveCriticalRegion(int tID)
{
	//printf("%d leave\n", tID);
	readyFlags[tID] = false;
}


void Counter(int count, int tID)
{
    for(int i=0; i < count; i++)
    {
		EnterCriticalRegion(tID);
        Count++;
		LeaveCriticalRegion(tID);
		Sleep(10);
    }
}

int sCounter = 0;
void Sum(void* pParams)
{
	int *c = (int *)pParams;
	for(int i=0; i < MAX; i++)
    {
		EnterCriticalSection(&cs);
        sCounter++;
		LeaveCriticalSection(&cs);
		Sleep(10);
    }
}

int main()
{
	std::thread th[N];
    for(int i = 0; i < N; i++)
    {
        th[i] = std::thread(Counter, MAX, i);
    }
    
    
	auto start = chrono::system_clock::now();
   
     for(int i = 0; i < N; i++)
    {
        th[i].join();
    }
    
	chrono::duration<double> duration = chrono::system_clock::now() - start;
	
	printf("Peterson Result: %d\n", Count);
	printf("Peterson Time: %lf\n\n", duration.count());
	
	InitializeCriticalSection(&cs);
	HANDLE hThreads_[N];

	start = chrono::system_clock::now();
	for(DWORD i = 0; i < N; i++)
        hThreads_[i] = (HANDLE)_beginthread(Sum, 0, (void *) (MAX));

	WaitForMultipleObjects(N, hThreads_, TRUE, INFINITE);
	duration = chrono::system_clock::now() - start;
	printf("WinAPI Result: %d\n", sCounter);
	printf("WinAPI Time (cs): %lf\n\n", duration.count());
	DeleteCriticalSection(&cs);
	system("pause");
    return 0;
}