#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

const int MAX_SIZE = 1000;

void sig_handler(int signum){
    exit(1);
}

int main(void)
{
    char inp[MAX_SIZE];
    int alarm_time;
    int status;

    pid_t pid = fork();

    if (pid == 0)
    {
        signal(SIGALRM, sig_handler);
        printf("Insert the alarm time: ");
        scanf("%d", &alarm_time);
        alarm(10);
        printf("Insert the string: ");
        scanf("%s", inp);
        printf("The inputted string size is %zu\n", strlen(inp));
        exit(0);
    }

    if (pid > 0){
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) == 1)
        {
            printf("You were too slow and this program will end!\n");
        }
    }

    return 0;
}
