#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For constants “mode” */
#include <fcntl.h>	  /* For constants O_* */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define CHILD_PROCESSES 8
#define TOTAL_NUMBERS 200

void writeNumbers(FILE *write)
{
	FILE *file_read;
	int num;

	// open Number.txt
	file_read = fopen("Numbers.txt", "r");

	if (file_read == NULL)
	{
		perror("Erro.No  Numbers.txt.\n");
		exit(1);
	}

	// efetuar leitura e escrita
	while (fscanf(file_read, "%d", &num) == 1)
	{
		fprintf(write, "[%d] %d\n", getpid(), num);
	}

	// fechar ficheiros não necessários
	fclose(file_read);
}

int main()
{

	pid_t p;
	FILE *output;
	FILE *write;
	sem_t *semaforo1, *semaforo2, *semaforo3, *semaforo4, *semaforo5, *semaforo6, *semaforo7, *semaforo8;
	char linha[100];

	sem_unlink("semaforo1"); // remove o semaforo se ja existir com este nome
	sem_unlink("semaforo2"); // remove o semaforo se ja existir com este nome
	sem_unlink("semaforo3"); // remove o semaforo se ja existir com este nome
	sem_unlink("semaforo4"); // remove o semaforo se ja existir com este nome
	sem_unlink("semaforo5"); // remove o semaforo se ja existir com este nome
	sem_unlink("semaforo6"); // remove o semaforo se ja existir com este nome
	sem_unlink("semaforo7"); // remove o semaforo se ja existir com este nome
	sem_unlink("semaforo8"); // remove o semaforo se ja existir com este nome

	// criação de semaforos
	if ((semaforo1 = sem_open("semaforo1", O_CREAT, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED)
	{
		perror("Erro na criação do semaforo.\n");
		exit(1);
	}
	if ((semaforo4 = sem_open("semaforo4", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
	{
		perror("Erro na criação do semaforo.\n");
		exit(1);
	}
	if ((semaforo5 = sem_open("semaforo5", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
	{
		perror("Erro na criação do semaforo.\n");
		exit(1);
	}
	if ((semaforo6 = sem_open("semaforo6", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
	{
		perror("Erro na criação do semaforo.\n");
		exit(1);
	}
	if ((semaforo7 = sem_open("semaforo7", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
	{
		perror("Erro na criação do semaforo.\n");
		exit(1);
	}
	if ((semaforo8 = sem_open("semaforo8", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
	{
		perror("Erro na criação do semaforo.\n");
		exit(1);
	}
	if ((semaforo2 = sem_open("semaforo2", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
	{
		perror("Erro na criação do semaforo.\n");
		exit(1);
	}
	if ((semaforo3 = sem_open("semaforo3", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
	{
		perror("Erro na criação do semaforo.\n");
		exit(1);
	}
	

	// remoção do ficheiro "Output.txt"
	if (remove("Output.txt") == -1)
	{
		perror("Ocorreu no ficheiro Output.txt.\n");
		exit(1);
	}

	// criação do ficheiro Output.txt
	write = fopen("Output.txt", "w");

	if (write == NULL)
	{
		perror("Erro. No Output.txt.\n");
		exit(1);
	}

	for (int i = 0; i < CHILD_PROCESSES; i++)
	{
		p = fork();

		// processo filho
		if (p == 0)
		{
			if (i == 0)
			{
				sem_post(semaforo1);
				writeNumbers(write);
				sem_post(semaforo2);
			}
			else if (i == 1)
			{
				sem_wait(semaforo2);
				writeNumbers(write);
				sem_post(semaforo3);
			}
			else if (i == 2)
			{
				sem_wait(semaforo3);
				writeNumbers(write);
				sem_post(semaforo4);
			}
			else if (i == 3)
			{
				sem_wait(semaforo4);
				writeNumbers(write);
				sem_post(semaforo5);
			}
			else if (i == 4)
			{
				sem_wait(semaforo5);
				writeNumbers(write);
				sem_post(semaforo6);
			}
			else if (i == 5)
			{
				sem_wait(semaforo6);
				writeNumbers(write);
				sem_post(semaforo7);
			}
			else if (i == 6)
			{
				sem_wait(semaforo7);
				writeNumbers(write);
				sem_post(semaforo8);
			}
			else if (i == 7)
			{
				sem_wait(semaforo8);
				writeNumbers(write);
			}

			exit(0);
		}
	}

	// esperar que todos os processos terminem
	for (int j = 0; j < CHILD_PROCESSES; j++)
	{
		wait(NULL);
	}

	// abrir o Output.txt
	output = fopen("Output.txt", "r");

	if (output == NULL)
	{
		perror("Erro.\n");
		exit(1);
	}

	printf("Ficheiro Output.txt\n");

	while (fscanf(output, "%99[^\n]\n", linha) == 1)
	{
		printf("%s\n", linha);
	}

	// fechar ficheiros não necessários
	fclose(write);
	fclose(output);

	return 0;
}
