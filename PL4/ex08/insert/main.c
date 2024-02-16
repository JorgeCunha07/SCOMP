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
    fd = shm_open("/pl4ex08", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    Recorded_Data *shared_data = mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (shared_data->index < 0)
    {
        shared_data->index = 0;
    }

    sem_wait(sem);

    Client *c = &(shared_data->saved_data[shared_data->index]);
    char aux_name[NAME_SIZE], aux_add[ADDR_SIZE];

    printf("Number of client:\n");
    scanf("%d", &(c->number));

    printf("Client name:\n");
    scanf("%s", aux_name);
    strcpy(c->name, aux_name);

    printf("Client address:\n");
    scanf("%s", aux_add);
    strcpy(c->address, aux_add);

    shared_data->index++;

    return 0;
}