#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int spawn_child(int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (fork() == 0)
        {
            return (i + 1) * 2;
        }
    }

    return 0;
}