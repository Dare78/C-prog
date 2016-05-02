// This is a simple program which uses fork to call 3 child process and increament the a variable each time the child is called


#include<stdio.h>
#include<stdlib.h>

main()
{
	int pid, pid_01, pid_02;
	pid = fork();
	int count=0;
	switch(pid)
{
	case 0:
	if(pid==0)
	printf ("Child01 with Process ID: %d\n increment variable %d\n", getpid(),++count);
	
	pid_01 = fork();
	if(pid_01 == 0)

{
	printf("child 02 with Process ID: %d\n increment variable %d\n", getpid(),++count);

	
	pid_02 = fork();
	if(pid_02==0)

{
	printf("child03 with Process ID: %d\n increment variable %d\n", getpid(),++count);

}

}
break;

case -1:
	printf("error occured");
break;

default:

	printf("parent process with ID %d\n",getpid());
	wait();

break;
}
}
