#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/wait.h>

int main(void)
{
    sem_t *sem;
    sem = sem_open("sem", O_CREAT, 0644, 1);
    if (sem == SEM_FAILED)
    {
        perror("Error in sem_open()");
        exit(1);
    }

    pid_t pid;

    pid = fork();

    if (pid == 0)
    {
        printf("1st child!\n");
        sem_post(sem);
        exit(0);
    }

    sem_wait(sem);
    wait(NULL);
    printf("Father!\n");

    pid = fork();

    if (pid == 0)
    {
        printf("2nd child!\n");
        sem_post(sem);
        exit(0);
    }

    sem_wait(sem);
    wait(NULL);

    if (sem_unlink("sem") < 0)
        exit(1);

    return 0;
}