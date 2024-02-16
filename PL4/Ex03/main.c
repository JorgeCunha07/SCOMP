#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h> 
#include <fcntl.h>	  


#define TOTAL_STRINGS 50
#define CHILD_PROCESSES 5

int main()
{
	struct timespec t;
	char string[80];
	char aux[40] = "I'm the Father - with PID ";
	int fd, fd_aux;
	int data_size = sizeof(string) * TOTAL_STRINGS;
	sem_t *sem;
	pid_t p;

	
	fd = shm_open("/Ex3", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR); //memória partilhada  para o armazenamento das strings
	ftruncate(fd, data_size);
	char(*strings)[TOTAL_STRINGS] = mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	
	fd_aux = shm_open("/Ex3_helper", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	ftruncate(fd_aux, sizeof(int));
	int *size = (int *)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd_aux, 0);// memória partilha para armazenamento auxiliar

	
	*(size) = 0;//numero de strings guardadas

	// criação do semáforo
	if ((sem = sem_open("semaforo", O_CREAT, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED){
		perror("Erro.\n");
		exit(1);
	}

	srand(time(NULL));

	// atribuição do tempo de espera (12 segundos);
	t.tv_sec = 12;

	for (int i = 0; i < CHILD_PROCESSES; i++)
	{
		p = fork();

		if (p == 0)
		{
			while (1)
			{

				// terminar programa se todas as strings forem escritas
				if (*(size) == 49)
				{
					exit(0);
				}

				// bloquear os restantes processos
				if (sem_timedwait(sem, &t) == -1)
				{
					printf("O tempo de espera foi ultrapassado\n");
					exit(1);
				}
				// gerar um número aleatório
				int r = rand() % 9;
				// remover última string com 30% de chance
				if (r <= 2){
					if (*(size) != 0){
						*(size)--;
					}
				}
				else{
					if (*(size) != 49){
						// adicionar string com 70% de chance
						sprintf(strings[*(size)], "%s %d", aux, getpid());
						*(size)++;
					}
				}

				sleep((rand() % 4) + 1); //para fazer sleep rand de 1 ate 5

				// desbloquear restantes processos
				sem_post(sem);
			}
		}
	}

	
	for (int j = 0; j < CHILD_PROCESSES; j++){
		wait(NULL);
	}//todos os processos filhos terminem

	
	for (int k = 0; k < TOTAL_STRINGS; k++){
		printf("%s%d\n", strings[k], *(size));
	//Apresenta todas as strings
	}

	return 0;
}
