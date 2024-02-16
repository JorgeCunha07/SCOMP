#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>

#define PATH_SIZE 1000
#define WORD_SIZE 10
#define CHILD_NUMBER 10

typedef struct
{
    char path[PATH_SIZE];
    char word[WORD_SIZE];
    int occurences;
} shared_data_type;

int count_word(char *f, char *word)
{
    int count = 0;
    FILE *file = fopen(f, "r");
    char line[100];

    while (fgets(line, sizeof(line), file))
    {
        char *find = word;
        int lineSize = strlen(line);
        int wordSize = strlen(word);
        int found;
        int i, j;

        for (i = 0; i <= lineSize - wordSize; i++)
        {
            found = 1;

            for (j = 0; j < wordSize; j++)
            {
                if (line[i + j] != find[j])
                {
                    found = 0;
                    break;
                }
            }

            if (found == 1)
            {
                count++;
            }
        }
    }

    return count;
}

int main(void)
{
    int fd, data_size = CHILD_NUMBER * sizeof(shared_data_type);
    shared_data_type *shared_data;
    fd = shm_open("/pl3ex08", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    pid_t pid[CHILD_NUMBER];
    int i;

    for (i = 0; i < CHILD_NUMBER; i++)
    {
        strcpy(shared_data[i].path, "find_words.txt");
        sprintf(shared_data[i].word, "word‰d", i);
    }

    for (i = 0; i < CHILD_NUMBER; i++)
    {
        pid[i] = fork();

        if (pid[i] == 0)
        {
            int occurrences = count_word(shared_data[i].path, shared_data[i].word);
            shared_data[i].occurences = occurrences;
            exit(0);
        }
    }

    wait(NULL);
    for (i = 0; i < CHILD_NUMBER; i++)
    {
        printf("The child number %d found %d ocurrences.\n", i + 1, shared_data[i].occurences);
    }

    if (munmap(shared_data, sizeof(shared_data_type)) != 0)
    {
        perror("Munmap error");
        exit(1);
    }

    if (shm_unlink("/pl3ex08") != 0)
    {
        perror("Unlink error");
        exit(1);
    }

    return 0;
}
