#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define ARRAY_SIZE 1000
#define SPLIT_SIZE 200

int main()
{
    int vec1[ARRAY_SIZE];
    int vec2[ARRAY_SIZE];
    int result[ARRAY_SIZE];
    int pipes[5][2], inicio[5], final[5];

    pid_t child_pid[5];

    // Initialize arrays with some values
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        vec1[i] = i;
        vec2[i] = i;
        result[i] = 0;
    }

    // Create pipes and child processes
    for (int i = 0; i < 5; i++)
    {
        if (pipe(pipes[i]) == -1)
        {
            perror("Erro ao criar o pipe");
            exit(1);
        }

        inicio[i] = i * SPLIT_SIZE;
        final[i] = inicio[i] + SPLIT_SIZE;

        if ((child_pid[i] = fork()) == -1)
        {
            perror("Erro ao criar o child process");
            exit(1);
        }

        if (child_pid[i] == 0)
        {
            // Child process
            close(pipes[i][0]);

            for (int j = inicio[i]; j < final[i]; j++)
            {
                int sum = vec1[j] + vec2[j];
                write(pipes[i][1], &sum, sizeof(int));
            }

            close(pipes[i][1]);
            exit(0);
        }
        else
        { // Parent process
            close(pipes[i][1]);

            for (int j = inicio[i]; j < final[i]; j++)
            {
                int sum;
                read(pipes[i][0], &sum, sizeof(int));
                result[j] = sum;
            }

            close(pipes[i][0]);
            waitpid(child_pid[i], NULL, 0);
        }
    }

    // Print result array
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        printf("Posicao -> %d valor -> %d\n", i + 1, result[i]);
    }
    printf("\n");

    return 0;
}
