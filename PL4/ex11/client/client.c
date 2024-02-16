#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>	  
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

#define CLIENTS 10

int main()
{
	pid_t pid;
	int fd;
	int data_size = sizeof(int);
	int val, r;
	sem_t *semaforo1, *sharedsemaforo, *sharedsemaforo2;
	srand(time(NULL));

	// Access Shared memory
	fd = shm_open("/shared", O_RDONLY, S_IRUSR | S_IWUSR);
	ftruncate(fd, data_size);

	int *ticket = (int *)mmap(NULL, data_size, PROT_READ, MAP_SHARED, fd, 0);

	// Semáforo syncronization between Clients
	// Semáforo comunication de Seller to Client
	// Semáforo comunication de Client to Seller
	if ((semaforo1 = sem_open("semaforo1", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
	{
		perror("Error.\n");
		exit(1);
	}
	if ((sharedsemaforo = sem_open("shared_semaforo", O_CREAT)) == SEM_FAILED)
	{
		perror("Error.\n");
		exit(1);
	}
	if ((sharedsemaforo2 = sem_open("shared_semaforo2", O_CREAT)) == SEM_FAILED)
	{
		perror("Error.\n");
		exit(1);
	}

	for (int i = 0; i < CLIENTS; i++)
	{

		pid = fork();

		if (pid == 0)
		{

			do
			{

				sem_getvalue(semaforo1, &val); // get Value of the semaforo

			} while (val != i); // while its not moved to next client

			sem_wait(sharedsemaforo); // Waits for the ticket to be avaible

			r = (rand() % 10) + 1;
			sleep(r); // Sleep between 1 to 11 seconds

			printf("Ticket -> %d  && Client -> %d  \n", *ticket, i + 1); // Print of the ticket  and the process

			sem_post(sharedsemaforo2); // Sold Ticket

			sem_post(semaforo1); // Permission for next Client

			exit(0);
		}
	}

	for (int j = 0; j < CLIENTS; j++)
	{
		wait(NULL);
	}

	return 0;
}
