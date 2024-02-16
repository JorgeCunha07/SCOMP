#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_SIZE 100

typedef struct
{
    int number;
    char msg[MAX_SIZE];
} info;

int main(void)
{
    int fd[2];
    int write_number;
    char write_msg[MAX_SIZE];
    info info;

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
        printf("Write the message you want to pass: ");
        scanf("%s", write_msg);

        strcpy(info.msg, write_msg);
        info.number = write_number;
        write(fd[1], &info, sizeof(info));

        close(fd[1]);
    }

    else if (pid == 0)
    {
        close(fd[1]);

        read(fd[0], &info, sizeof(info));
        printf("The integer I got from the father process is: %d\n", info.number);
        printf("The string I got from the father process is: %s\n", info.msg);

        close(fd[0]);
        exit(0);
    }

    return 0;
}
