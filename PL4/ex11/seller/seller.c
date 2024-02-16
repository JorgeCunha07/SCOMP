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

#define TICKETS 100


int main() { 
	int fd;
	int data_size = sizeof(int);
	int tickets[TICKETS];
	sem_t *sharedsemaforo, *sharedsemaforo2;
	
	srand(time(NULL));
	
	
	
	for(int i = 0; i < TICKETS; i++) { 
		tickets[i] = i + 1;  //Inicialize number of tickets
	}
	
	//Create shared memory
	fd = shm_open("/shared", O_CREAT|O_TRUNC|O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(fd, data_size);
	int *ticket = (int *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	

	// Semáforo comunication de Seller to Client
	// Semáforo comunication de Client to Seller
	if((sharedsemaforo = sem_open("shared_semaforo", O_CREAT, S_IRUSR|S_IWUSR, 0)) == SEM_FAILED) { 
		perror("Error.\n");
		exit(1);
	}
	
	//criar o semáforo para a comunicação de cliente para vendedor
	if((sharedsemaforo2 = sem_open("shared_semaforo2", O_CREAT, S_IRUSR|S_IWUSR, 0)) == SEM_FAILED) { 
		perror("Error.\n");
		exit(1);
	}
	
	
	
	for(int j = 0; j < TICKETS; j++) {
	
		*ticket = tickets[j];// send ticket to shared memory
		


		sem_post(sharedsemaforo);// communicate client that the ticket may be avaible
		
	
		sem_wait(sharedsemaforo2);// wait for it to be sold
	}
	
	return 0;
}
