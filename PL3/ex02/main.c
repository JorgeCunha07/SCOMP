#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define ITERATIONS 1000000

typedef struct
{
    int n1;
    int n2;
} shared_data_type;

int main(void)
{
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;
    fd = shm_open("/pl3ex02", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    shared_data->n1 = 10000;
    shared_data->n2 = 500;

    int i, j;
    pid_t pid = fork();

    if (pid == 0)
    {
        for (j = 0; j < ITERATIONS; j++)
        {
            shared_data->n1++;
            shared_data->n2--;
        }
        exit(0);
    }

    for (i = 0; i < ITERATIONS; i++)
    {
        shared_data->n1--;
        shared_data->n2++;
    }

    wait(NULL);
    printf("The value 10000 is now: %d\n", shared_data->n1);
    printf("The value 500 is now: %d\n", shared_data->n2);

    if (munmap(shared_data, sizeof(shared_data_type)) != 0)
    {
        perror("Munmap error");
        exit(1);
    }

    if (shm_unlink("/pl3ex02") != 0)
    {
        perror("Unlink error");
        exit(1);
    }

    return 0;
}