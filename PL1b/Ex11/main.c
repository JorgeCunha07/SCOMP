#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int signum)
{
    sigset_t set;
    int i;

    // Get the current signal mask
    sigprocmask(SIG_BLOCK, NULL, &set);

    printf("Signals Blocked: \n");
    for (i = 1; i < NSIG; i++)
    {
        if (sigismember(&set, i))
        {
            printf("%d \n", i);
        }
    }

    // Reset the signal mask to the previous state
    sigprocmask(SIG_SETMASK, &set, NULL);
}

int main()
{
    struct sigaction salp;

    // Register the signal handler for SIGUSR1

    salp.sa_handler = handler;

    sigemptyset(&salp.sa_mask);

    salp.sa_flags = 0;

    sigaction(SIGUSR1, &salp, NULL);

    // Block all signals except SIGUSR1

    sigset_t mask;

    sigfillset(&mask);

    sigdelset(&mask, SIGUSR1);

    sigprocmask(SIG_SETMASK, &mask, NULL);

    printf("Waiting for the signal !");

    pause();

    return 0;
}

/*
• CTRL-C sends SIGINT;
• CTRL-\ sends SIGQUIT;
• CTRL-Z sends SIGTSTP.
    command stty -a.
    ps -ef | grep ex09
    kill -SIGINT 1234
  or   kill -INT 1234
*/
