#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define CHILD_NUMBER 10

typedef struct
{
    char msg[4];
    int round_number;
} pipe_game;

int main(void)
{
    int fd[2];
    int i;
    int status;
    int counter = 1;
    int round;
    pid_t pid[CHILD_NUMBER];
    pipe_game pipe_game;

    if (pipe(fd) == -1)
    {
        perror("Pipe failed");
        return 1;
    }

    for (i = 0; i < CHILD_NUMBER; i++)
    {
        pid[i] = fork();

        if (pid[i] == 0)
        {
            close(fd[1]);
            read(fd[0], &pipe_game, sizeof(pipe_game));
            printf("%s\n%d\n", pipe_game.msg, pipe_game.round_number);
            close(fd[0]);
            printf("%d\n", getpid());
            exit(pipe_game.round_number);
        }
    }

    for (i = 0; i < CHILD_NUMBER; i++)
    {
        close(fd[0]);
        strcpy(pipe_game.msg, "Win");
        pipe_game.round_number = counter;
        write(fd[1], &pipe_game, sizeof(pipe_game));
        counter++;
        sleep(2);
    }

    printf("\n");

    for (i = 0; i < CHILD_NUMBER; i++)
    {
        wait(&status);
        round = WEXITSTATUS(status);
        printf("The process %d is the winner of the round %d!\n", pid[i], round);
    }

    close(fd[1]);

    return 0;
}
