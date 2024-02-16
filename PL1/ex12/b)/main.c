#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "spawn_child.h"

const int CHILDS_NUMBER = 6;

int main(void)
{
   int status;
   int childs = spawn_child(CHILDS_NUMBER);

    wait(&status);

    if (WEXITSTATUS(status))
    {
        printf("Child number is %d\n", childs);
    }

    if (childs == CHILDS_NUMBER * 2)
    {
        printf("Ending father process number 0\n");
    }

    return 0;
}