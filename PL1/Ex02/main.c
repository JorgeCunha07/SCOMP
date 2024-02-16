#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void)
{
    int status;
    pid_t pid = fork();

    if (pid > 0)
    {
        printf("I'm..\n");
    }
    else
    {
        wait(NULL);
        printf("I'll never join you!\n");

        pid = fork();

        if (pid > 0)
        {
            printf("the..\n");
        }
        else
        {
            wait(NULL);
            printf("I'll never join you!\n");

            pid = fork();

            if (pid > 0)
            {
                printf("father!\n");
            }
            else
            {
                wait(NULL);
                printf("I'll never join you!\n");
            }
        }
    }

    waitpid(pid, &status, 0);

    return 0;
}
