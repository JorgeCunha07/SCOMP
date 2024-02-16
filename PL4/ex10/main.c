#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define N_PASSENGERS 20
#define N_DOORS 3

#define STARTING_EXCHANGES 15
#define EXCHANGES_A 15
#define EXCHANGES_B 15

typedef struct
{
    int passengers;
    int exchanges;
} Train;

Train *train;

sem_t *semC;
sem_t *semS;
sem_t *semM;

void openDoors()
{
    int i;
    for (i = 0; i < N_DOORS; i++)
    {
        sem_post(semC);
    }
}

void nExits(int numExits)
{
    int i;
    for (i = 0; i < numExits; i++)
    {
        sem_post(semS);
    }
}

void passenger(int id)
{
    printf("Passenger %d is waiting.\n", id);

    sem_wait(semC);
    sem_wait(semM);
    if (train->passengers == N_PASSENGERS)
    {
        sem_post(semM);
        sem_post(semC);
        exit(0);
    }
    else
    {
        printf("Passenger %d is riding the train\n", id);
        train->passengers++;
        train->exchanges++;
        sem_post(semM);
    }
    usleep(200000);
    sem_post(semC);
    sem_wait(semS);
    sem_wait(semC);

    printf("Passenger %d left the train.\n", id);

    sem_wait(semM);

    train->passengers--;
    train->exchanges++;

    sem_post(semM);
    usleep(200000);
    sem_post(semC);

    exit(0);
}

void atStation(int exchanges)
{
    int nExchanges;

    do
    {
        sem_wait(semM);
        nExchanges = train->exchanges;
        sem_post(semM);
        usleep(200000);
    } while (nExchanges != exchanges);

    sem_wait(semM);
    train->exchanges = 0;
    sem_post(semM);
}

int main(void)
{
    int fd, data_size = sizeof(Train);
    fd = shm_open("/pl4ex10", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    train = (Train *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    semC = sem_open("semC", O_CREAT, 0644, 0);
    semS = sem_open("semS", O_CREAT, 0644, 0);
    semM = sem_open("semM", O_CREAT, 0644, 0);

    train->passengers = 0;
    int i;

    sem_post(semM);
    openDoors();
    for (i = 1; i < 16; i++)
    {
        if (fork() == 0)
        {
            passenger(i);
        }
    }

    atStation(STARTING_EXCHANGES);

    printf("\nTrain departs with 15 passengers\n\n");

    usleep(200000);
    printf("Train arrives at station A\n\n");
    openDoors();
    nExits(5);

    for (i = 16; i < 36; i++)
    {
        if (fork() == 0)
        {
            passenger(i);
        }
    }

    atStation(EXCHANGES_A);

    printf("\nTrain departs\n\n");

    usleep(200000);
    printf("Train arrives at station B\n\n");
    openDoors();
    nExits(10);

    for (i = 36; i < 41; i++)
    {
        if (fork() == 0)
        {
            passenger(i);
        }
    }

    atStation(EXCHANGES_B);

    printf("\nTrain departs\n\n");
    usleep(200000);
    printf("Train arrives to final destination\n\n");
    openDoors();
    nExits(N_PASSENGERS);

    int numPassengers;
    do
    {
        sem_wait(semM);
        numPassengers = train->passengers;
        sem_post(semM);
        usleep(200000);
    } while (numPassengers != 0);

    sem_close(semC);
    sem_close(semS);
    sem_close(semM);

    if (sem_unlink("semC") < 0)
    {
        perror("Unlink semC error");
        exit(1);
    }

    if (sem_unlink("semS") < 0)
    {
        perror("Unlink semS error");
        exit(1);
    }

    if (sem_unlink("semM") < 0)
    {
        perror("Unlink semM error");
        exit(1);
    }

    if (munmap(train, data_size) != 0)
    {
        perror("Munmap error");
        exit(1);
    }

    if (shm_unlink("/pl4ex10") != 0)
    {
        perror("Unlink mem error");
        exit(1);
    }

    return 0;
}
