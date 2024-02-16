#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <limits.h>

int main()
{
    int pid[5];
    int i, fd[6][2], valorPai, valorFilho;
    time_t t;
    srand((unsigned)time(&t));

    // Create pipes
    for (i = 0; i < 6; i++)
    {
        if (pipe(fd[i]) == -1)
        {
            perror("Erro ao criar o pipe");
            exit(1);
        }
    }

    for (i = 0; i < 5; i++)
    {
        pid[i] = fork();

        if (pid[i] == -1)
        {
            perror("Erro ao criar o child process");
            exit(1);
        }

        else if (pid[i] == 0)
        { // Child process

            close(fd[i][1]);
            // Close write end of the pipe

            read(fd[i][0], &valorPai, sizeof(int));
            close(fd[i][0]);
            // Close read end of the pipe

            valorFilho = rand() % 501;

            printf("Filho : %d -> valor %d\n", getpid(), valorFilho);
            if (valorFilho < valorPai)
            {
                valorFilho = valorPai;
            }

            if (i != 4)
            {
                close(fd[i + 1][0]); // Close read end of the pipe
                write(fd[i + 1][1], &valorFilho, sizeof(int));
                close(fd[i + 1][1]); // Close write end of the pipe
                exit(0);
            }
            else if (i == 4)
            {
                close(fd[i + 1][0]); // Close read end of the pipe
                write(fd[i + 1][1], &valorFilho, sizeof(int));
                close(fd[i + 1][1]); // Close write end of the pipe
                exit(0);
            }
        }
        else
        { // Parent process
            if (i == 0)
            {
                valorPai = rand() % 501;
                printf("Pai : %d -> valor %d\n", getpid(), valorPai);
                close(fd[i][0]); // Close read end of the pipe
                write(fd[i][1], &valorPai, sizeof(int));
                close(fd[i][1]); // Close write end of the pipe
            }
            if (i == 4)
            {
                close(fd[i + 1][1]); // Close read end of the pipe
                read(fd[i + 1][0], &valorPai, sizeof(int));
                close(fd[i + 1][0]); // Close write end of the pipe
                printf("Greatest number %d\n", valorPai);
            }
        }
    }

    return 0;
}