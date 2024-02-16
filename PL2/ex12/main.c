#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define NAME_SIZE 10
#define BARCODE_READERS 5

typedef struct
{
    int barcode_info;
    char product_name[NAME_SIZE];
    float price;
} Product_info;

int main(void)
{
    int fd[2];
    int shared_fd[2];
    int i, j;
    int reading;
    pid_t pid;
    Product_info product_info[BARCODE_READERS];
    Product_info aux_product_info;

    for (i = 0; i < BARCODE_READERS; i++)
    {
        product_info[i].barcode_info = i;
        product_info[i].price = i + 0.99;
        strcpy(product_info[i].product_name, "Product");
    }

    if (pipe(shared_fd) == -1)
    {
        perror("Pipe failed");
        return 1;
    }

    for (i = 0; i < BARCODE_READERS; i++)
    {
        if (pipe(fd) == -1)
        {
            perror("Pipe failed");
            return 1;
        }

        pid = fork();

        if (pid == 0)
        {
            srand(getpid());
            reading = rand() % 5;
            printf("PRODUCT REQUEST %d\n", reading);
            write(shared_fd[1], &reading, sizeof(int));

            if (i == BARCODE_READERS - 1)
            {
                close(shared_fd[0]);
                close(shared_fd[1]);
            }

            sleep(1);
            close(fd[1]);
            read(fd[0], &aux_product_info, sizeof(aux_product_info));
            printf("Product name: %s\nProduct price: %.2f\n\n", aux_product_info.product_name, aux_product_info.price);
            close(fd[0]);

            exit(1);
        }

        else if (pid > 0)
        {
            read(shared_fd[0], &reading, sizeof(int));

            printf("PRODUCT REQUEST IN PARENT %d\n", reading);
            for (j = 0; j < BARCODE_READERS; j++)
            {
                if (product_info[j].barcode_info == reading)
                {
                    close(fd[0]);
                    write(fd[1], &product_info[j], sizeof(aux_product_info));
                    close(fd[1]);
                }
            }
            wait(NULL);
        }
    }

    close(shared_fd[0]);
    close(shared_fd[1]);

    return 0;
}
