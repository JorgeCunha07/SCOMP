#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For constants “mode” */
#include <fcntl.h> /* For constants O_* */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define CHILD_PROCESSES 6

void buy_chips() { 
	printf("Buying chips!\n");
}

void buy_beer() { 
	printf("Buying beer!\n");
}

void eat_and_drink() { 
	printf("Eating and drinking!\n");
}

int main() { 
	sem_t *sem;
	pid_t p;
	int sval = 0;
	
	srand(time(NULL));// permitir o rand funcionar
	
	//criação do semáforo
	if((sem = sem_open("semaforo", O_CREAT, S_IRUSR|S_IWUSR, 0)) == SEM_FAILED) { 
		perror("Erro.\n");
		exit(1);
	}
	
	for(int i = 0; i < CHILD_PROCESSES; i++) { 
		p = fork();
	
		if(p == -1) { 
			perror("Erro no fork.\n");
			exit(1);
		}
		
		if(p == 0) {
			//gerar um número aleatório (tempo para o comando "sleep" e para decidir a execução de uma das funções)
			int r = rand() % 6;
		
			//bloquear durante uns segundos
			sleep(r);
			
			//decidir entre a execução de buy_beer() ou buy_chips() (com base no último número aleatório)
			if(r < 3) { 
				buy_chips();
				buy_beer();
			} else { 
				buy_beer();
				buy_chips();
			}
			
			//incrementar o semáforo
			sem_post(sem);
		
			//bloquear até que todos os processos executem as funções buy_beer() ou buy_chips()
			do{ 
				if(sem_getvalue(sem, &sval) == -1) {
					perror("Valor de semaforo ERROR . /n");
					exit(1);
				}
			}while (sval != 6);
			
			
			//finalmente executar a função eat_and_drink()
			eat_and_drink();
			
			exit(0);
		}
	}
	
	//esperar que todos os processos filho terminem
	for(int j = 0; j < CHILD_PROCESSES; j++) { 
		wait(NULL);
	}
	
	return 0;
}
