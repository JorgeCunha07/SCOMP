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


int main() { 
	sem_t *sem1;     
	sem_t *sem2;
	pid_t p;
	
	srand(time(NULL));
	int random = rand() % 10;

	if((sem1 = sem_open("semaforo1", O_CREAT, S_IRUSR|S_IWUSR, 0)) == SEM_FAILED) { 
		perror("Erro\n");
		exit(1);
	}
	if((sem2 = sem_open("semaforo2", O_CREAT, S_IRUSR|S_IWUSR, 0)) == SEM_FAILED) { 
		perror("Erro\n");
		exit(1);
	}
	
	p = fork();
	
	if(p == -1) { 
		perror("Ocorreu um erro na criação do processo filho!\n");
		exit(1);
	}
	

	sleep(random);

	if(p > 0) { 
		
		printf("buying chips!\n");//processo pai
	
		sem_post(sem2);//incrementar semáforo,processo filho continua
		
		
		sem_wait(sem1);//esperar que o processo filho termine
		
	} else { 
		
		printf("buying beer!\n");//processo filho
		sem_post(sem1);//incrementar semáforo ,processo pai continua
		sem_wait(sem2); //esperar que o processo pai termine
	}
	
	if(p > 0) { 
		printf("1 Eating and drinking!\n");
	} else { 
		printf("P2 Eating and drinking!\n");
		exit(0);
	}
	
	wait(NULL);
	
	return 0;
}
