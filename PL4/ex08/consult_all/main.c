#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <stdbool.h>
#include <time.h>

#define NAME_SIZE 50
#define ADDR_SIZE 100
#define MAX_RECS 100

typedef struct
{
    int number;
    char name[NAME_SIZE];
    char address[ADDR_SIZE];
} Client;

typedef struct
{
    Client saved_data[MAX_RECS];
    int index;
} Recorded_Data;

int main(void)
{

    sem_t *sem = sem_open("/sem", O_CREAT, 0644, 1);
    int fd, data_size = sizeof(Recorded_Data);
    fd = shm_open("/pl4ex08", O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    Recorded_Data *shared_data = mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sem_wait(sem);

    int i;

    if (shared_data->index == 0)
    {
        printf("No data found\n");
    }
    else
    {
        for (i = 0; i < shared_data->index; i++)
        {
            Client *c = &(shared_data->saved_data[i]);
            printf("Client info:\n");
            printf("Number: %d \nName: %s\nAddress: %s\n\n", c->number, c->name, c->address);
        }
    }

    sem_post(sem);

    if (sem_unlink("sem") < 0)
    {
        perror("Unlink sem error");
        exit(1);
    }

    if (munmap(shared_data, data_size) != 0)
    {
        perror("Munmap error");
        exit(1);
    }

    return 0;
}