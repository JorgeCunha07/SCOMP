/* ps os processos a serem executados no terminal exec [init]
    ps -ef|grep todos processos que podem correr na maquina 
    kill -l signals*/
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid;
    int sig_num;

    // Get PID and signal number from user
    printf("Enter PID : ");
    scanf("%d", &pid);

    printf("Enter signal number: ");
    scanf("%d", &sig_num);

    // Send signal to process
    if (kill(pid, sig_num) == -1) {
        perror("kill");
        exit(EXIT_FAILURE);
    }

    printf("Signal %d process %d\n", sig_num, pid);

    return 0;
}

    

