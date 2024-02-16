#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void handle_USR1(int signo)
{
write(STDOUT_FILENO, "\n I won t let the processend by pressing CTRL-\ ! \n", 13);
}


int main()
{
    struct sigaction act1,act2;
    /* Clear the act variable. */
    memset(&act1, 0, sizeof(struct sigaction));
    act1.sa_handler = SIG_IGN;
    sigaction(SIGINT, &act1, NULL);

    memset(&act2, 0, sizeof(struct sigaction));
    sigemptyset(&act2.sa_mask); /* No signals blocked */
    act2.sa_handler = handle_USR1;
    sigaction(SIGQUIT, &act2, NULL);

    for (;;)
    {
        printf("I like signals \n ");
        sleep(1);
    }

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
