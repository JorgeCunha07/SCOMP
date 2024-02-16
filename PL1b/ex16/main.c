#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

#define CHILD_NUMBER 50

volatile sig_atomic_t finished_childs = 0;
volatile sig_atomic_t successful_childs = 0;

int simulate1()
{
    srand(getpid());
    int n = rand() % 5 + 1;
    if (n > 4)
        return 1;

    return 0;
}

void simulate2()
{
    printf("GOT SIGUSR2!\n");
    exit(0);
}

void sig_handler1(int signum, siginfo_t *sinfo, void *context)
{
    if (signum == SIGUSR1)
    {
        finished_childs++;
        successful_childs++;
    }
    else if (signum == SIGUSR2)
        finished_childs++;
}

void sig_handler2(int signum, siginfo_t *sinfo, void *context)
{
    if (signum == SIGUSR1)
    {
        simulate2();
    }
}

int main()
{
    pid_t pid[CHILD_NUMBER];
    int i, j;
    int signal_return;
    struct sigaction act;

    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = sig_handler1;

    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);

    for (i = 0; i < CHILD_NUMBER; i++)
    {
        pid[i] = fork();

        if (pid[i] == 0)
        {   
            act.sa_sigaction = sig_handler2;
            sigaction(SIGUSR1, &act, NULL);

            signal_return = simulate1();
            sleep(1);

            if (signal_return == 1)
                kill(getppid(), SIGUSR1);
            
            else
                kill(getppid(), SIGUSR2);

            pause();
        }
    }

    while (finished_childs < 25)
    {
        pause();
    }

    if (successful_childs > 0)
    {
        for (j = 0; j < CHILD_NUMBER; j++)
            kill(pid[j], SIGUSR1);
    }
    else
    {
        printf("Inefficient algorithm!\n");

        for (j = 0; j < CHILD_NUMBER; j++)
            kill(pid[j], SIGKILL);
    }

    wait(NULL);

    return 0;
}