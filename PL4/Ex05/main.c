#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define CHILD_PROCESSES 3

int main()
{
	sem_t *semaforo1, *semaforo2, *semaforo3;
	pid_t pid;

	// criação de semaforos
	if ((semaforo1 = sem_open("semaforo1", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
	{
		perror("Erro!\n");
		exit(1);
	}
	if ((semaforo2 = sem_open("semaforo2", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
	{
		perror("Erro!\n");
		exit(1);
	}
	if ((semaforo3 = sem_open("semaforo3", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
	{
		perror("Erro!\n");
		exit(1);
	}

	for (int i = 0; i < CHILD_PROCESSES; i++)
	{
		pid = fork();

		if (pid == 0)
		{
			if (i == 0)
			{
				printf("Sistemas ");
				fflush(stdout);
				sem_post(semaforo2); // permite os outros processos

				sem_wait(semaforo1); // bloqueia os processos
				printf("a ");
				sem_post(semaforo2);
				exit(0);

			}
			else if (i == 1)
			{   
				sem_wait(semaforo2); // bloqueia os processos
				printf("de ");
				fflush(stdout);
				sem_post(semaforo3); // permite os outros processos

				sem_wait(semaforo2); // bloqueia os processos
				printf("melhor ");
				sem_post(semaforo3); // permite os outros processos

				exit(0);
			}
			else if (i == 2)
			{ 	
				sem_wait(semaforo3); // bloqueia os processos
				printf("Computadores - ");
				fflush(stdout);
				sem_post(semaforo1); // permite os outros processos

			
				sem_wait(semaforo3); // bloqueia os processos
				printf("disciplina! ");
		
				exit(0);
			}
		}
	}

	sem_close(semaforo1);
    sem_close(semaforo2);
    sem_close(semaforo3);
    sem_unlink("semaforo1");
    sem_unlink("semaforo2");
    sem_unlink("semaforo3");

	return 0;
}