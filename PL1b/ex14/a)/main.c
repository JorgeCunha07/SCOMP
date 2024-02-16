#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

const int MAX_SIZE = 1000;

void sig_handler(int signum)
{
    printf("You were too slow and this program will end!\n");
    exit(0);
}

int main(void)
{
    char inp[MAX_SIZE];

    signal(SIGALRM, sig_handler);
    alarm(10);
    printf("Insert the string: ");
    scanf("%s", inp);
    printf("The inputted string size is %zu\n", strlen(inp));

    return 0;
}
