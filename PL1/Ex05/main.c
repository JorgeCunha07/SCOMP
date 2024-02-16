#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void)
{
    int status1,status2 ;
    pid_t pid1 = fork();
    pid_t pid2;
    waitpid(pid1,&status1,0);/*The parent process waits for the child to finish the process */
    if (pid1 == 0){
        sleep(1);/*sleeps for 1 sec */
        exit(1);/*Terminates with code 1*/
                  
    }else{
     pid2 = fork();

    waitpid(pid2,&status2,0);/*The parent process waits for the child to finish the process */
    }
    if (pid2 == 0){
    sleep(2);/*sleeps for 2 sec */
    exit(2);/*Terminates with code 2*/
    }
    int x = WEXITSTATUS(status1);/*The exist status gives the value of the process exited*/
    int y = WEXITSTATUS(status2);/*The exist status gives the value of the process exited*/
    printf("status 1 :%d  status 2 : %d \n ",x,y);

    return 0;
}
