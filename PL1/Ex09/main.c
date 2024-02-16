#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(void) {

    int status;

    for(int i=0; i<10; i++) {
        pid_t processo = fork();
        if(processo==0){/*For each fork the child counts 100 times the number after the last child*/
            for(int j = (i*100) + 1; j<= (i*100) + 100; j++){
            printf("numero %d \n", j);
            }
            printf("\n");
            exit(1); 
        }
        if (processo > 0)
        {/*For each fork the parent waits for the child to terminate before starting the next if*/
            wait(NULL);
        }
        
    }
    /*In this case the output is sorted because of the if on line 21 that waits for each child process to terminate before beginning the next one. */
    /*In this case the output is guarented because the parent process waits for the child process to finish.*/

}
