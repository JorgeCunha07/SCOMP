#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void)
{
    int i;
    int status;

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
        }
        
    }
    printf("This is the end.\n");

    return 0;
}