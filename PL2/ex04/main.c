#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_SIZE 1000

int main(void)
{
    FILE *ptr;
    int fd[2];
    int status;
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

        // Opening file in reading mode
        ptr = fopen("test.txt", "r");

        if (NULL == ptr)
        {
            printf("File can't be opened or doesn't exist!\n");
            exit(0);
        }

        while (fgets(write_msg, MAX_SIZE, ptr))
        {
            write(fd[1], write_msg, strlen(write_msg) + 1);
            sleep(1);
        }

        close(fd[1]);
        wait(&status);
    }

    else if (pid == 0)
    {
        close(fd[1]);

        while (read(fd[0], write_msg, MAX_SIZE))
        {
            printf("%s", write_msg);
        }

        close(fd[0]);
        exit(0);
    }

    return 0;
}
