#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <string.h>

void handle_Alarm(int sig)
{
    char warning[230];
    sprintf(warning, "The command %d did not complete in its allowed time! Exf3 \n", getpid());
    write(STDOUT_FILENO, warning, strlen(warning));
    kill(getpid(), SIGKILL);

}

int main(int argc, char *argv[])

{
        char *time_cap_str = argv[1]; // Get the string from the command-line argument
    printf("time_cap_str: %s\n", time_cap_str);
    
           char array[10];
        struct sigaction act;
        memset(&act, 0, sizeof(struct sigaction));
        sigemptyset(&act.sa_mask);
        act.sa_handler = handle_Alarm;
        sigaction(SIGALRM, &act, NULL);

        alarm(atoi(argv[1]));
    int i;

    for (i = 0; i < 10; i++)
    {
        printf("%d\n ", i);
    }

    printf("Exf3 Done! \n");
}
