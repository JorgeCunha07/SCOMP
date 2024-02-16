#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>

int TAMANHO = 1000;
int CHILDREN = 5;

int main()
{

    int numbers[TAMANHO], result[TAMANHO], maximoTotal = 0, status;
    time_t t;

    /* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand((unsigned)time(&t));

    /* initialize array with random numbers (rand(): stdlib.h) */

    for (int i = 0; i < TAMANHO; i++)
    {
        numbers[i] = rand() % 256;
    }

    for (int i = 0; i < CHILDREN; i++)
    {
        pid_t processo = fork();
        int maximo = 0, valor;
        if (processo == 0)
        {
            valor = TAMANHO / 5;
            for (int j = i * valor; j < ((i + 1) * valor); j++)
            {
                if (maximo < numbers[j])
                {
                    maximo = numbers[j];
                }
            }
            exit(maximo);
        }
        else
        {
            /*For each fork the parent waits for the child to terminate before starting the next if*/
            wait(&status);
            if (WIFEXITED(status))
            {
                if (maximoTotal < WEXITSTATUS(status))
                {
                    maximoTotal = WEXITSTATUS(status);
                }
            }
        }
    }
    pid_t processo2 = fork();
    if (processo2 == 0)
    {
        for (int r = 0; r < TAMANHO / 2; r++)
        {
            result[r] = (int)((numbers[r] * 100) / maximoTotal);
        }
        for (int r = 0; r < TAMANHO / 2; r++)
        {
            printf("Posição -> %d valor : %d \n", r + 1, result[r]);
        }
    }
    else
    {
        for (int f = TAMANHO / 2; f < TAMANHO; f++)
        {
            result[f] = (int)((numbers[f] * 100) / maximoTotal);
        }
        wait(NULL);
        /*both child and parent process must perform the computation concurrently, but the output must be sorted bythe array indexes.*/
        for (int f = TAMANHO / 2; f < TAMANHO; f++)
        {
            printf("Posição -> %d valor : %d \n", f + 1, result[f]);
        }
    }
    return 0;
}
