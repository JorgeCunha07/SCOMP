#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define NUMBER_STRINGS 50
#define STRING_SIZE 80

typedef struct
{
    char strings[NUMBER_STRINGS][STRING_SIZE];
    int index;
} shared_data_type;

int main(void)
{
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;
    fd = shm_open("/pl4ex02", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    sem_t *sem;
    sem = sem_open("sem", O_CREAT, 0644, 1);
    if (sem == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(1);
    }

    shared_data->index = 0;

    pid_t pid = fork();

    if (pid == 0)
    {
        while (1)
        {
            if (shared_data->index == NUMBER_STRINGS)
            {
                sem_post(sem);
                exit(1);
            }

            sprintf(shared_data->strings[shared_data->index], "I'm the Father - with PID %d\n", getppid());
            printf("%s", shared_data->strings[shared_data->index]);
            shared_data->index++;
        }
    }

    sem_wait(sem);

    if (munmap(shared_data, sizeof(shared_data_type)) < 0)
        exit(1);

    if (shm_unlink("/pl4ex02") < 0)
        exit(1);

    if (sem_unlink("sem") < 0)
        exit(1);

    return 0;
}