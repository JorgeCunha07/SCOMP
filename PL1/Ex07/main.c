#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define ARRAY_SIZE 1000
int main()
{
    int total = 0;
    int status;
    int numbers[ARRAY_SIZE]; /* array to lookup */
    int n;                   /* the number to find */
    time_t t;                /* needed to init. the random number generator (RNG) */
    int i;

    /* intializes RNG (srand():stdlib.h; time(): time.h) */
    srand((unsigned)time(&t));

    /* initialize array with random numbers (rand(): stdlib.h) */
    for (i = 0; i < ARRAY_SIZE; i++){
        numbers[i] = rand() % 10000;
    }
    /* initialize n */
    n = rand() % 10000;

    pid_t pid = fork();

    if (pid < 0)
    {/* if there is an error creating a child for the process */
        printf("Error");
        exit(-1);
    }

    if (pid == 0)
    {/* The child process reads half of the array to  check if the number rand is visible on half of it and if so how many times*/
        int value1 = 0;
        for (i = 0; i < ARRAY_SIZE / 2; i++)
        {
            if (n == numbers[i])
            {
                value1++;
            }
        }
        printf("valor filho %d\n", value1);
        exit(value1);
    }
    else
    {
        for (i = ARRAY_SIZE / 2; i < ARRAY_SIZE; i++)
        {/* The Parent process reads half of the array to  check if the number rand is visible on half of it and if so how many times */
            if (n == numbers[i])
            {
                total++;
            }
        }
        printf("Valor Pai %d\n", total);

        waitpid(pid, &status, 0);

        total = total + WEXITSTATUS(status);
        printf("\nO valor total de vezes é %d e o numero é %d\n", total, n);
    }
    /* Doubt 
    if (WEXITSTATUS(status))
    {

        total = total + WEXITSTATUS(status);
        printf("\nO valor total de vezes é %d e o numero é %d\n", total, n);
    }
    */
}