#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void)
{
    int i;
    int status;
    int counter = 0;

    for (i = 0; i < 2; i++)
    {
        int pid = fork();

        if (pid > 0 && getpid() % 2 == 0 )
        {
            waitpid(pid, &status, 0);
        }
        
        if (pid == 0)
        {  
            sleep(1);
            counter++;
            printf("This is the child number %d, and parent pid is %d\n", counter, getppid());
        }
        
    }
    printf("This is the end.\n");

    return 0;
}