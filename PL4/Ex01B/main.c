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
	pid_t pid;
	FILE* file_read;
	FILE* file_write;
	sem_t *read,*write;
	int number;
	
    sem_unlink("read");  // remove existing semaphore with the same name
    sem_unlink("write");  // remove existing semaphore with the same name
	
	//criação do semáforo 
	if((read = sem_open("read", O_CREAT, S_IRUSR|S_IWUSR, 1)) == SEM_FAILED) { 
		perror("Erro no sem_open()\n");
		exit(1);
	}
    
    if((write = sem_open("write", O_CREAT, S_IRUSR|S_IWUSR, 1)) == SEM_FAILED) { 
		perror("Erro no sem_open()\n");
		exit(1);
	}

	//remoção do ficheiro "Output.txt"
	if(remove("Output.txt") == -1) { 
		perror("Ocorreu no ficheiro Output.txt\n");
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
				
                //bloquea os processos de fazerem a leitura
                sem_wait(read);

				if(fscanf(file_read, "%d", &number) == 1) { 
					
                    //permite que os outros processos que leiam
					sem_post(read);
					
					//bloquea os processos de fazerem a escrita
					sem_wait(write);

					fprintf(file_write, "Pid : %d -> %d\n", getpid(), number);
					
					//permite que os outros processos que escrevam
					sem_post(write);
					 
				} else {
					
                    //desbloquear os processos para a leitura no término de forma a permitir que eles terminem a sua execução 
					sem_post(read); 
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
	
    file_read = fopen("Output.txt", "r");

    char linha[100];
    printf("Leitura do Ficheiro:\n");

    while (fscanf(file_read, "%99[^\n]\n", linha) == 1)
    {
        
        printf("%s\n",linha);
    }
    
    fclose(file_read);
	return 0;
}
