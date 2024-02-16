#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

void foo()
{
    char msg[80];

    sprintf(msg, "I captured a SIGUSR1 sent by the process with PID %d\n", getpid());
    write(STDOUT_FILENO, msg, strlen(msg));
}

int main(void)
{
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);

    act.sa_handler = foo;
    sigaction(SIGUSR1, &act, NULL);

    return 0;
}
