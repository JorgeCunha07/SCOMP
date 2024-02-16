#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_SIZE 100

int main(void)
{
    int fd[2];
    int write_number;
    char write_msg[MAX_SIZE];

    if (pipe(fd) == -1)
    {
        perror("Pipe failed");
        return 1;
    }
    
    pid_t pid = fork();

    if (pid > 0)
    {
        close(fd[0]);

        printf("Write the number you want to pass: ");
        scanf("%d", &write_number);
        write(fd[1], &write_number, sizeof(int));

        sleep(0.5);
        printf("Write the message you want to pass: ");
        scanf("%s", write_msg);
        write(fd[1], write_msg, strlen(write_msg) + 1);

        close(fd[1]);
    }

    else if (pid == 0)
    {
        close(fd[1]);

        read(fd[0], &write_number, sizeof(int));
        printf("The integer I got from the father process is: %d\n", write_number);
        
        read(fd[0], write_msg, MAX_SIZE);
        printf("The string I got from the father process is: %s\n", write_msg);

        close(fd[0]);
        exit(0);
    }

    return 0;
}
