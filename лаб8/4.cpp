#include <windows.h>
#include <thread>
#include <stdio.h>

int SUM = 0;

void sum(int N)
{
    for(int i = 0; i < N; i++)
    {
        SUM++;
    }
    //printf("%d\n", SUM);
}

int main()
{
    std::thread th1(sum, 1000);
    std::thread th2(sum, 3000);
    std::thread th3(sum, 10000);
   
    th1.join();
    th2.join();
    th3.join();
    printf("%d\n", SUM);
    return 0;
}