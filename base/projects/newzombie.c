#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int pid;
	pid=fork();
	if(pid>0)	/* Child Process */
	{
		sleep(1000);
	}
	else		/* Parent Process */
	{
		exit(0);
	}
}