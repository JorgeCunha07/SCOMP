#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_SIZE 1000
#define CHILD_NUMBER 5

int main(void)
{
    int fd[2];
    int i, j;
    int tmp, total = 0;
    int status;
    int vec1[MAX_SIZE];
    int vec2[MAX_SIZE];
    pid_t pid;

    if (pipe(fd) == -1)
    {
        perror("Pipe failed");
        return 1;
    }

    for (i = 0; i < MAX_SIZE; i++)
    {
        vec1[i] = i;
        vec2[i] = i * 10;
        total += vec1[i] + vec2[i];
    }

    printf("%d\n", total);

    for (i = 0; i < CHILD_NUMBER; i++)
    {   
        tmp = 0;
        pid = fork();
        
        if (pid > 0)
        {
            waitpid(pid, &status, 0);
        }

        else if (pid == 0)
        {
            close(fd[0]);

            for (j = i * 200; j < (i * 200) + 200; j++)
            {
                tmp += vec1[j] + vec2[j];
            }

            write(fd[1], &tmp, sizeof(int));
            close(fd[1]);
            exit(0);
        }
    }
    
    int res = 0;
    close(fd[1]);         
    for (i = 0; i < CHILD_NUMBER; i++)
    {
        read(fd[0], &tmp, sizeof(int));
        res += tmp;
    }
    
    printf("%d\n", res);
    close(fd[0]);

    return 0;
}
