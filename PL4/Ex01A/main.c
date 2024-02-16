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

#define CHILD_PROCESSES 8
#define TOTAL_NUMBERS 200


int main() { 
    
    //sem_unlink("sema");  // remove existing semaphore with the same name
	pid_t pid;
	FILE* file_read;
	FILE* file_write;
	sem_t *sem;
	int number;
	
	
	//criação do semáforo
	if((sem = sem_open("sema", O_CREAT, S_IRUSR|S_IWUSR, 1)) == SEM_FAILED) { 
		perror("Erro no sem_open()\n");
		exit(1);
    
	}
	file_write = fopen("Output.txt", "w");
	file_read = fopen("Numbers.txt", "r");
			
	if(file_read == NULL|| file_write == NULL ) { 
		perror("Erro. Não foi possível abrir ou criar o ficheiro. \n");
		exit(1);
	}
	
	for(int i = 0; i < CHILD_PROCESSES; i++) { 
		pid= fork();
		
		//processo filho
		if(pid == 0) { 
			while(1) {
				
				//decrementar o semáforo 
				sem_wait(sem);
				
				//Verificação se existe alguma linha para leitura
				if(fscanf(file_read, "%d", &number) == 1) { 

                    //Se existir escreve para o ficheiro
					fprintf(file_write, "Pid : %d -> %d\n", getpid(), number);
					
					//incrementar semáforo 
					sem_post(sem); 
				} else { 
					
					//desbloquear os outros processos caso não haja mais nada para ler, de forma a permitir que eles terminem
					sem_post(sem); 
					exit(0);
				}
			}
		}
	}
	
	for(int j = 0; j < CHILD_PROCESSES; j++) { 
		wait(NULL);
	}
	
	//fechar ficheiros 
	fclose(file_read);
	fclose(file_write);
	

	return 0;
}
