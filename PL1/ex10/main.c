#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

const int SIZE = 2000;

int main(void)
{
    int array_ints[SIZE];
    int i, j;
    int n = 0;
    int status;
    int return_array[10];

    for (i = 0; i < SIZE; i++)
    {
        array_ints[i] = 0;
    }

    for (i = 0; i < 10; i++)
    {
        pid_t pid = fork();

        if (pid == 0)
        {
            for (j = (i * 200); j < (i * 200) + 200; j++)
            {
                if (array_ints[j] == n)
                {
                    exit(j);
                } 
            }
            exit(255);

        }else{
            waitpid(pid, &status, 0);

            return_array[i] = WEXITSTATUS(status);
        }
    }

    for (i = 0; i < 10; i++)
    {
        if (return_array[i] != 255)
        {
            printf("%d\n", return_array[i]);
        }
        
    }
    

    return 0;
}