#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

int main(void)
{
    int fd[2], credits_fd[2];
    int status;
    int generated_number;
    int bet_status;
    int child_bet;
    int child_credits = 20;
    pid_t pid;

    if (pipe(fd) == -1)
    {
        perror("Pipe failed");
        return 1;
    }

    if (pipe(credits_fd) == -1)
    {
        perror("Pipe failed");
        return 1;
    }

    pid = fork();

    if (pid == 0)
        close(credits_fd[1]);
    else if (pid > 0)
        close(credits_fd[0]);

    while (1)
    {
        if (pid > 0)
        {
            srand(time(NULL));
            generated_number = (rand() % 5) + 1;
            printf("GENERATED NUMBER %d\n", generated_number);

            if (child_credits > 0)
                bet_status = 1;
            else
                bet_status = 0;

            write(fd[1], &bet_status, sizeof(int));

            if (bet_status == 0)
            {
                waitpid(pid, &status, 0);
                close(fd[0]);
                close(fd[1]);
                close(credits_fd[1]);
                exit(0);
            }

            sleep(1);
            read(fd[0], &child_bet, sizeof(int));

            printf("The child bet is %d\n", child_bet);
            printf("The generated number is %d\n\n", generated_number);

            if (child_bet == generated_number)
                child_credits += 10;
            else
                child_credits -= 5;

            write(credits_fd[1], &child_credits, sizeof(int));
            sleep(1);
        }

        else if (pid == 0)
        {
            read(fd[0], &bet_status, sizeof(int));
            printf("BET STATUS %d\n", bet_status);

            if (bet_status == 0)
            {
                close(fd[0]);
                close(fd[1]);
                close(credits_fd[0]);
                exit(0);
            }

            srand(time(NULL));
            child_bet = (rand() % 5) + 1;
            
            printf("BETTING %d\n", child_bet);
            write(fd[1], &child_bet, sizeof(int));
            sleep(1);

            read(credits_fd[0], &child_credits, sizeof(int));
            printf("I have %d euros!\n\n", child_credits);
            sleep(1);
        }
    }

    return 0;
}
