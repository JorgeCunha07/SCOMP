#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <string.h>

volatile sig_atomic_t flag = 0;

void handle_SIG(int signo)
{
	//	char msg[80];
	flag = 1;
	//	sprintf(msg, "Job is complete!\n"); Se usar direto no write nao funciona porque ?
	//	write(STDOUT_FILENO, msg, strlen(msg));
}

int main(void)
{

	int state;
	time_t t;
	srand((unsigned)time(&t));

	struct sigaction act;
	sigemptyset(&act.sa_mask); /* No signals blocked */
	act.sa_handler = handle_SIG;
	sigaction(SIGUSR1, &act, NULL);

	pid_t pid = fork();
	if (pid > 0)
	{
		printf("Task A: started!\n");
		sleep(3);
		printf("Task A: done!\n");
		kill(pid, SIGUSR1);
		waitpid(pid, &state, 0);
		printf("The job is complete!\n");
	}
	else
	{ /*Se usar o wait pid pelo pai nao funciona de maneira correta.*/
		while (flag != 1)
		{
			pause();
		}

		int randomNumber;
		randomNumber = rand() % 6;

		printf("Task B: started!\n");
		sleep(randomNumber);
		printf("Task B: done!\n");

		printf("Task C: started!\n");
		sleep(1);
		printf("Task C: done!\n");

		exit(0);
	}
}
