#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

volatile sig_atomic_t USR1_counter = 0;

void handle_USR1(int signo, siginfo_t *sinfo, void *context)
{
    char msg[80];
    USR1_counter++;
    sprintf(msg, "SIGUSR signal captured: USR1 counter = %d\n", USR1_counter);
    write(STDOUT_FILENO, msg, strlen(msg));
    sleep(1);
}

int main(void)
{
    int i;
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigfillset(&act.sa_mask);

    act.sa_sigaction = handle_USR1;
    sigaction(SIGUSR1, &act, NULL);
    struct timespec remaining, request = {1, 1};

    pid_t pid = fork();

    if (pid == 0)
    {
        for (i = 0; i < 12; i++)
        {
            nanosleep(&request, &remaining);
            kill(getppid(), SIGUSR1);
        }
        kill(getppid(), SIGINT);
        exit(1);
    }

    while (1)
    {
        printf("I'm working!\n");
        sleep(1);
    }

    return 0;
}
