#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define ITERATIONS 30

typedef struct
{
    int buffer[10];
    int size;
    int producer;
    int consumer;
} shared_data_type;

shared_data_type *shared_data;
sem_t *sem;

int producer(void)
{
    int i;
    for (i = 0; i < ITERATIONS; i++)
    {
        sem_wait(sem);
        while (shared_data->size == 10)
        {
            sem_post(sem);
            usleep(200000);
            sem_wait(sem);
        }

        printf("Producer writes: %d\n", i);
        shared_data->buffer[shared_data->consumer] = i;
        shared_data->consumer = (shared_data->consumer + 1) % 10;
        shared_data->size++;
        sem_post(sem);
        sleep(1);
    }
    return 0;
}

int consumer(void)
{
    int flag;
    while (flag > 0)
    {
        sleep(2);
        sem_wait(sem);
        while (shared_data->size == 0)
        {
            sem_post(sem);
            usleep(200000);
            sem_wait(sem);
        }
        printf("\nConsumer reads: %d\n\n", shared_data->buffer[shared_data->producer]);
        shared_data->producer = (shared_data->producer + 1) % 10;
        shared_data->size--;
        flag = shared_data->size;
        sem_post(sem);
    }

    return 0;
}

int main(void)
{
    int fd, data_size = sizeof(shared_data_type);
    fd = shm_open("/pl4ex12", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sem = sem_open("sem", O_CREAT, 0644, 1);

    shared_data->consumer = 0;
    shared_data->producer = 0;
    shared_data->size = 0;

    pid_t pid = fork();

    if (pid == 0)
    {
        producer();
        exit(0);
    }

    pid = fork();

    if (pid == 0)
    {
        producer();
        exit(0);
    }

    consumer();

    sem_close(sem);

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

    if (close(fd) != 0)
    {
        perror("Pipe error");
        exit(1);
    }

    if (shm_unlink("/pl4ex12") != 0)
    {
        perror("Unlink mem error");
        exit(1);
    }

    return 0;
}
