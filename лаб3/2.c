#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void oldman()
{
	fprintf(stdout, "I'm not yet dead! My ID is %i\n",(int)getpid());
}

void recreation()
{
	fprintf(stdout, "Who I am? My ID is %i\n",(int)getpid());
}

void createChildProcess()
{
	pid_t child_pid = fork();
	if (child_pid != 0)
	{
		fprintf(stdout, "%u spawned %u\n", getpid(), child_pid);
	}
	else
	{
		return;
	}
}

int main()
{
	pid_t child_pid, parent_pid;
	int i = 0;
	fprintf(stdout, "Before RECREATION %i\n", parent_pid = (int) getpid());
	child_pid = fork();
	createChildProcess();
	createChildProcess();
	getchar();
	//kill(parent_pid, SIGTERM);
	return 0;
}