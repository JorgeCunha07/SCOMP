#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define N_VALUES 10

sem_t *semM, *semAX, *semBX;

typedef struct
{
    int data[N_VALUES];
    int index;
} shared_data_type;
shared_data_type *shared_data;

void AX(void)
{
    int i, rValues[N_VALUES], indexP;

    while (1)
    {
        for (i = 0; i < N_VALUES; i++)
        {
            rValues[i] = rand() % 20 + 1;
        }
        sem_wait(semAX);

        sem_wait(semM);
        indexP = shared_data->index;
        if (indexP > N_VALUES - 1)
        {
            sem_post(semM);
            sem_post(semAX);
            break;
        }

        printf("Index %d\n", indexP);
        printf("Value %d\n", rValues[indexP]);

        shared_data->data[indexP] = rValues[indexP];
        shared_data->index++;

        sem_post(semM);
        sleep(1);
        sem_post(semAX);
        sleep(5);
    }

    exit(EXIT_SUCCESS);
}

void BX(void)
{
    int i, rValues[N_VALUES], indexP;

    while (1)
    {
        for (i = 0; i < N_VALUES; i++)
        {
            rValues[i] = rand() % 20 + 1;
        }
        sem_wait(semBX);

        sem_wait(semM);
        indexP = shared_data->index;
        if (indexP > N_VALUES - 1)
        {
            sem_post(semM);
            sem_post(semBX);
            break;
        }

        printf("Index %d\n", indexP);
        printf("Value %d\n", rValues[indexP]);

        shared_data->data[indexP] = rValues[indexP];
        shared_data->index++;
        sem_post(semM);

        sleep(1);
        sem_post(semBX);

        sleep(6);
    }

    exit(EXIT_SUCCESS);
}

int main(void)
{
    int fd, data_size = sizeof(shared_data_type);
    int fd = shm_open("/pl4ex14", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    semM = sem_open("semM", O_CREAT, 0644, 0);
    semAX = sem_open("semAX", O_CREAT, 0644, 0);
    semBX = sem_open("semBX", O_CREAT, 0644, 0);

    shared_data->index = 0;

    sem_post(semAX);
    sem_post(semBX);
    sem_post(semM);

    int i;
    pid_t pid;

    for (i = 0; i < 5; i++)
    {
        pid = fork();

        if (i < 3 && pid == 0)
        {
            AX();
        }
        else if (pid == 0)
        {
            BX();
        }
    }

    int flag;
    do
    {
        sem_wait(semM);
        flag = shared_data->index;
        sem_post(semM);
        sleep(1);
    } while (flag < N_VALUES - 1);

    for (i = 0; i < N_VALUES; i++)
    {
        printf("%d, ", shared_data->data[i]);
    }

    sem_close(semM);
    sem_close(semAX);
    sem_close(semBX);

    if (sem_unlink("semM") < 0)
    {
        perror("Unlink semM error");
        exit(1);
    }

    if (sem_unlink("semAX") < 0)
    {
        perror("Unlink semAX error");
        exit(1);
    }

    if (sem_unlink("semBX") < 0)
    {
        perror("Unlink semBX error");
        exit(1);
    }

    if (munmap(shared_data, data_size) != 0)
    {
        perror("Munmap error");
        exit(1);
    }

    if (shm_unlink("/pl4ex14") != 0)
    {
        perror("Unlink mem error");
        exit(1);
    }

    return 0;
}