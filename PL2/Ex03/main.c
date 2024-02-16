#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
    int status;
    int tunnel1[2], tunnel2[2];

    char rapido1[20], rapido2[20];
    char resposta1[20], resposta2[20];

    strcpy(rapido1, "Hello World\n");
    strcpy(rapido2, "Goodbye!\n");

    pipe(tunnel1);
    pipe(tunnel2);

    pid_t pid = fork();
    if (pid > 0)
    {
        close(tunnel1[0]);
        close(tunnel2[0]);

        write(tunnel1[1], &rapido1, sizeof(rapido1));
        close(tunnel1[1]);

        write(tunnel2[1], &rapido2, sizeof(rapido2));
        close(tunnel2[1]);

        wait(&status);
        printf("Child Pid -> %d\nstatus -> %d \n", pid, WEXITSTATUS(status));
    }
    else
    {
        close(tunnel1[1]);
        close(tunnel2[1]);

        read(tunnel1[0], &resposta1, sizeof(resposta1));
        printf("%s", resposta1);
        close(tunnel1[0]);

        read(tunnel2[0], &resposta2, sizeof(resposta1));
        printf("%s", resposta2);
        close(tunnel2[0]);

        exit(7);
    }
}