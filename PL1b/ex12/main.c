#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

const int CHILDS = 5;
volatile sig_atomic_t childs_counter = CHILDS;

void handle_USR1(int signo, siginfo_t *sinfo, void *context)
{
    childs_counter--;
}

int main(void)
{
    int i, j;
    int status;
    pid_t pid[CHILDS];

    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    act.sa_flags = SA_NOCLDWAIT | SA_NOCLDSTOP;
    act.sa_sigaction = handle_USR1;
    sigaction(SIGUSR1, &act, NULL);

    for (i = 0; i < CHILDS; i++)
    {
        pid[i] = fork();

        if (pid[i] == 0)
        {
            for (j = i * 200; j < (i + 5) * 200; j++)
            {
                printf("%d\n", j);
            }
            kill(getppid(), SIGUSR1);
            exit(1);
        }
        else if (pid[i] > 0)
        {
            pause();
            printf("The number of active childs processes is: %d\n", childs_counter);
        }
    }

    return 0;
}
