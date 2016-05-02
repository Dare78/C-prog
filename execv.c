//this is a code which uses fork and use EXECVE to create a new process by calling the summation.c

#include<stdio.h>
#include<stdlib.h>

void main(int argc, char *argv[])
{
        int pid = 0;
        char *indentifier[] = {"Sumitup"};
        pid = fork(); /* Child process initiated */
        wait();
        if(pid == 0)
        {
                printf("Child Process created and its PID = %d \n The Summation is : \n", getpid());
                execve("summation", indentifier, NULL);
}
                
}
