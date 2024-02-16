#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>

#define MAXIMUM_SIZE 256

void stringConverter(char *frase)
{
    for (unsigned long i = 0; i < strlen(frase); i++)
    {
        if (frase[i] >= 'a' && frase[i] <= 'z')
        {
            frase[i] -= 32;
        }

        else if (frase[i] >= 'A' && frase[i] <= 'Z')
        {
            frase[i] += 32;
        }
    }
}

int main()
{
    int up[2];
    int down[2];
    char escrita[MAXIMUM_SIZE];
    char rescrita[MAXIMUM_SIZE];

    if (pipe(up) == -1 || (pipe(down) == -1))
    {
        perror("Error processing pipe up or down.\n");
        return -1;
    }

    int id = fork();

    if (id == 0)
    {
        close(up[1]);

        read(up[0], rescrita, sizeof(rescrita));

        stringConverter(rescrita);

        close(up[0]);
        close(down[0]);
        write(down[1], rescrita, sizeof(rescrita));
        close(down[1]);
        exit(2);
    }

    else if (id > 0)
    {
        close(up[0]);
        close(down[1]);

        printf("Message -> \n");
        scanf("%s", escrita);

        write(up[1], escrita, sizeof(escrita));
        close(up[1]);

        read(down[0], rescrita, sizeof(rescrita));
        printf("New Message -> %s \n", rescrita);

        close(down[0]);
        exit(0);
    }

    return 0;
}
