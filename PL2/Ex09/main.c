#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <limits.h>

#define ARRAY_SIZE 50000
#define STARTER_SIZE 5000

typedef struct
{
    int customer_code;
    int product_code;
    int quantity;
} produtos;

int main()
{

    time_t t;
    srand((unsigned)time(&t));
    int i, fd[10][2];
    produtos array[ARRAY_SIZE];
    int products[ARRAY_SIZE], num_products = 0;

    // Add random values to the structure
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        produtos temp = {
            .customer_code = rand() % INT_MAX,
            .product_code = rand() % INT_MAX,
            .quantity = rand() % 25};
        array[i] = temp;
    }

    // Create pipes
    for (i = 0; i < 10; i++)
    {
        if (pipe(fd[i]) == -1)
        {
            perror("Erro ao criar o pipe");
            exit(1);
        }
    }

    // Create 10 child processes
    for (i = 0; i < 10; i++)
    {
        pid_t pid = fork();

        if (pid == -1)
        {
            perror("Erro ao criar o child process");
            exit(1);
        }
        else if (pid == 0)
        { // Child process

            close(fd[i][0]);
            // Close reading end of the pipe

            int inicio = i * STARTER_SIZE;
            int final = inicio + STARTER_SIZE;

            for (int j = inicio; j < final; j++)
            {
                if (array[j].quantity > 20)
                {
                    write(fd[i][1], &array[j].product_code, sizeof(int));
                }
            }

            close(fd[i][1]); // Close writing end of the pipe
            exit(0);
        }
        else
        { // Parent process

            close(fd[i][1]); // Close writing end of the pipe

            int product_code;

            while (read(fd[i][0], &product_code, sizeof(int)) > 0)
            {
                products[num_products++] = product_code;
            }

            close(fd[i][0]);
            // Close reading end of the pipe
            wait(NULL);
            // Wait for child process to terminat
        }
    }

    // Print products array

    printf("Produtos que venderam mais de 20 unidades:\n");

    for (i = 0; i < num_products; i++)
    {
        printf("Produto -> %d\n", products[i]);
    }

    return 0;
}
