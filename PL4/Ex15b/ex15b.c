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

#define MAX_CAPACITY 10
#define CLIENTS 20
#define VIP_CLIENT 1
#define SPECIAL_CLIENT 2
#define NORMAL_CLIENT 3

typedef struct
{
	int total;
	int created;
	int vip;
	int special;
	int normal;
} info;

int main()
{
	pid_t p;
	int fd;
	int data_size = sizeof(info);
	int typeClient;
	sem_t *semaforo1, *semaforo2, *sem_Shield, *sem_Shield_Vip, *sem_Shield_normal, *sem_Shield_data;

	srand(time(NULL));

	// shared Memory
	fd = shm_open("/sharedMemory", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
	ftruncate(fd, data_size);
	info *clientsInfo = (info *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	// initializer data
	clientsInfo->total = 0;
	clientsInfo->created = 0;
	clientsInfo->vip = 0;
	clientsInfo->special = 0;
	clientsInfo->normal = 0;

	// Creation semaforo Clients in
	// Creation semaforo Clients OUT
	// Creation semaforo Shield
	// Creation semaforo Shield VIP
	// Creation semáforo Shield for normal Clients to not go in
	if ((semaforo1 = sem_open("sem_clientsIN", O_CREAT, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED)
	{
		perror("Error.\n");
		exit(1);
	}
	if ((semaforo2 = sem_open("sem_clientsOut", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
	{
		perror("Error.\n");
		exit(1);
	}

	if ((sem_Shield = sem_open("sem_Shield", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
	{
		perror("Error.\n");
		exit(1);
	}

	if ((sem_Shield_Vip = sem_open("sem_ShieldVip", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
	{
		perror("Error.\n");
		exit(1);
	}

	if ((sem_Shield_normal = sem_open("sem_ShieldNormal", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
	{
		perror("Error.\n");
		exit(1);
	}
		if ((sem_Shield_data = sem_open("sem_Shield_data", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
	{
		perror("Error.\n");
		exit(1);
	}


	// Creation of Clients
	for (int i = 0; i < CLIENTS; i++)
	{
		p = fork();

		if (p == 0)
		{
			// Random Value to chose the type of client
			typeClient = (rand() % 2) + 1;

			sem_wait(semaforo1);
			clientsInfo->created += 1;
			if (typeClient == NORMAL_CLIENT)
			{
				clientsInfo->normal += 1;
			}

			if (typeClient == SPECIAL_CLIENT)
			{
				clientsInfo->special += 1;
			}
			if (typeClient == VIP_CLIENT)
			{
				clientsInfo->vip += 1;
			}

			sem_post(semaforo1);

			// Shield while not every one is created
			if (clientsInfo->created == CLIENTS)
			{
				sem_post(sem_Shield);
			}

			// wait on the shield
			sem_wait(sem_Shield);

			// Open the shield
			sem_post(sem_Shield);

			// lock the clients
			if (typeClient == SPECIAL_CLIENT && clientsInfo->vip != 0)
			{
				sem_wait(sem_Shield_Vip);
				sem_post(sem_Shield_Vip);
			}

			if (typeClient == NORMAL_CLIENT && (clientsInfo->vip != 0 || clientsInfo->special != 0))
			{
				sem_wait(sem_Shield_normal);
				sem_post(sem_Shield_normal);
			}
			sem_wait(semaforo1);

			// clock begin
			clock_t begin = clock();

			clientsInfo->total += 1;

			if (typeClient == VIP_CLIENT)
			{
				printf("Vip Client %d: I'am in!\n", i + 1);
				clientsInfo->vip -= 1;

				// liberar a entrada de clientes VIP quando permitido
				if (clientsInfo->vip == 0)
				{
					sem_post(sem_Shield_Vip);
				}
			}
			else if (typeClient == SPECIAL_CLIENT)
			{
				printf("Special Client %d: I'am in!\n", i + 1);
				clientsInfo->special -= 1;

				// open normal clients
				if (clientsInfo->normal == 0)
				{
					sem_post(sem_Shield_normal);
				}
			}
			else if (typeClient == NORMAL_CLIENT)
			{
				printf("Normal Client %d: I'am in!\n", i + 1);
				clientsInfo->normal -= 1;
			}

			// allow  clients to go out
			if (clientsInfo->total == MAX_CAPACITY)
			{
				sem_post(semaforo2);
			}
			else
			{
				// allow clients to go in
				sem_post(semaforo1);
			}
			sem_wait(semaforo2);

			// Total time

			clock_t end = clock();
			double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

			printf("Client %d: Out\nTime: %f\n", i + 1, time_spent);
			clientsInfo->total -= 1;
			sem_post(semaforo1);

			exit(0);
		}
	}

	for (int j = 0; j < CLIENTS; j++)
	{
		wait(NULL);
	}

	return 0;
}
