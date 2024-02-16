#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

int main(void)
{
    sem_t *semP, *semC;
    semP = sem_open("semP", O_CREAT, 0644, 1);
    semC = sem_open("semC", O_CREAT, 0644, 1);

    pid_t pid = fork();

    if (pid == 0)
    {
        while (1)
        {
            sem_wait(semC);
            printf("C");
            fflush(stdout);
            sem_post(semP);
        }
        exit(0);
    }

    while (1)
    {
        sem_wait(semP);
        printf("S");
        fflush(stdout);
        sem_post(semC);
    }

    if (sem_unlink("semP") < 0)
        exit(1);
    
    if (sem_unlink("semC") < 0)
        exit(1);

    return 0;
}