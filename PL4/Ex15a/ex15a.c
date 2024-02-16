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

#define MAX 10
#define CLIENT 20

int main() { 
	pid_t p;
	int fd, fd2;
	int data_size = sizeof(int);
	sem_t *semaforoIn, *semaforoOut;
	
	
	//Shared Memory Total Inside
	fd = shm_open("/sharedMemoryTotal", O_CREAT|O_TRUNC|O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd, data_size);
	int *mCapacity = (int *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	*mCapacity = 0;

	//Shared Memory Next to come in

	fd2 = shm_open("/sharedMemoryIn", O_CREAT|O_TRUNC|O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd2, data_size);
	int *inNow = (int *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd2, 0);
	
	
	*inNow = 0;
	
	//Semaforo Creation Clients In
	//Semaforo Creation Clients Out

	if((semaforoIn = sem_open("clientsIn", O_CREAT, S_IRUSR|S_IWUSR, 1)) == SEM_FAILED) { 
		perror("Error.\n");
		exit(1);
	}
	
	
	if((semaforoOut = sem_open("clientsOut", O_CREAT, S_IRUSR|S_IWUSR, 0)) == SEM_FAILED) { 
		perror("Error.\n");
		exit(1);
	}
	
	
	
	//Clients with the define in global variable
	for(int i = 1; i <= CLIENT; i++) {
		p = fork();
		
		if(p == 0) { 
			
			while(*inNow != i - 1);
			
			if(*mCapacity == MAX) {
				//client Out
				sem_post(semaforoOut);
			}
			
			//waiting to get in
			sem_wait(semaforoIn);
			
			//Up m capacity
			printf("Client %d\n -> I'am In! \n", i);
			*mCapacity += 1;
			*inNow += 1;
			
			// allow next to go in
			sem_post(semaforoIn);
			
			//wait to get out
			sem_wait(semaforoOut);
			
			printf("Client %d\n -> I'am Out! \n", i);
			*mCapacity -= 1;
			
			
			//allow next to go in
			sem_post(semaforoIn);
			
			exit(0);
		}
	}
	
	for(int j = 0; j < CLIENT; j++) { 
		wait(NULL);
	}
	
	return 0;
}
