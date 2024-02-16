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
#include <string.h>

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		perror("São esperados 2 argumentos\n");
		exit(1);
	}

	pid_t p;
	int total_childs = atoi(argv[1]), sval = 0, time_spent;
	int exec_time = atoi(argv[2]);
	struct timeval start, end;
	sem_t *sem = sem_open("sema", O_CREAT, S_IRUSR | S_IWUSR, 0);

	gettimeofday(&start, NULL);

	for (int i = 0; i < total_childs; i++)
	{
		p = fork();

		if (p == 0)
		{
			usleep(exec_time);
			sem_post(sem);

			exit(0);
		}
	}

	while (sval != total_childs)
	{
		if (sem_getvalue(sem, &sval) == -1)
		{
			perror("Erro\n");
			exit(1);
		}
	}

	gettimeofday(&end, NULL);
	time_spent = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
	printf("Tempo decorrido: %d\n ms", time_spent);

	return 0;
}
