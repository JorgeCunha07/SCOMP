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

#define TWRITERS 4
#define TREADERS 4

typedef struct
{
	int readers;
	int writers;
	int canRead;
} information;

int main()
{
	pid_t p;
	char string[30];
	clock_t start, end;
	int fd, fd2, dataSize_info = sizeof(information), dataSize_string = sizeof(string);

	sem_t *semaforoString, *semaforoInformation;

	// create Shared memory
	fd = shm_open("/info", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	ftruncate(fd, dataSize_info);

	information *info = (information *)mmap(NULL, dataSize_info, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	info->canRead = 0;
	info->writers = 0;
	info->readers = 0;

	// create shared memory
	fd2 = shm_open("/string", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	ftruncate(fd2, dataSize_string);
	string = (char *)mmap(NULL, dataSize_string, PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);

	// Semaforo for String
	// Semaforo for the Struct
	if ((semaforoString = sem_open("semaforoString", O_CREAT, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED)
	{
		perror("Error.)\n");
		exit(1);
	}

	if ((semaforoInformation = sem_open("semaforoInfo", O_CREAT, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED)
	{
		perror("Error.\n");
		exit(1);
	}

	// Writers
	for (int i = 0; i < TWRITERS; i++)
	{
		p = fork();

		// Start the clock
		start = clock();

		if (p == 0)
		{
			// Block the access to the String
			sem_wait(semaforoString);

			// End the clock
			end = clock();

			double duration = ((double)end - start) / CLOCKS_PER_SEC;

			sprintf(string, "PID: %d | Time: %f");

			// increment
			info->canRead += 1;

			// unlock access to the String
			sem_post(semaforoString);
		}
	}

	// processos readers
	for (int j = 0; j < TREADERS; j++)
	{
		p = fork();

		if (p == 0)
		{
			// sleep 1 seconde
			sleep(1);

			// wait until the can read different of 0
			while (info->canRead == 0)
				;

			// read tne string
			char newString[30];
			strcpy(newString, string);

			sem_wait(semaforoInformation);
			info->readers += 1;
			printf("id -> %d ,string -> %s \n", j + 1, string);
			printf("Readers -> %d \n");

			sem_post(semaforoInformation);

			exit(0);
		}
	}
}
