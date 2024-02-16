#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <string.h>

typedef struct {
    char cmd[32]; // Command name.
    int time_cap; // Time limit to complete (in seconds).
} command_t;

int main()
{
    command_t c[3];
    strcpy(c[0].cmd, "./Exf1");
    strcpy(c[1].cmd, "./Exf2");
    strcpy(c[2].cmd, "./Exf3");

    c[0].time_cap = 10;
    c[1].time_cap = 1;
    c[2].time_cap = 20;

    int pro;
    for (pro = 0; pro < 3; pro++)
    {
        int pid = fork();

        if (pid == 0)
        {
            char time_cap_str[10]; // Create a buffer to hold the time limit as a string.
            sprintf(time_cap_str, "%d", c[pro].time_cap); // Convert the time limit to a string.

            execl(c[pro].cmd, c[pro].cmd, time_cap_str, NULL); // Pass the time limit string as a separate argument.
            perror("execl"); // Print an error message if execl() fails.
            exit(1);
        }else{
			wait(NULL);
		}
    }


    return 0;
}
