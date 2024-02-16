#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STR_SIZE 50
#define NR_DISC 10

typedef struct
{
    int numero;
    char nome[STR_SIZE];
    int disciplinas[NR_DISC];
    int flag;
} Aluno;

int main()
{
    Aluno *shared_data;
    int fd, data_size = sizeof(Aluno);

    // Create and open shared memory area
    shm_unlink("/ex7");
    fd = shm_open("/ex7", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        perror("shm_open error");
        exit(EXIT_FAILURE);
    }

    // Define size of shared memory area
    if (ftruncate(fd, data_size) == -1)
    {
        perror("ftruncate error");
        exit(EXIT_FAILURE);
    }

    // Map shared memory area in the process address space
    shared_data = mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_data == MAP_FAILED)
    {
        perror("mmap error");
        exit(EXIT_FAILURE);
    }

    // Set flag to 0 to indicate shared data is not yet available
    shared_data->flag = 0;

    pid_t p;
    p = fork();

    if (p < 0)
    {
        perror("Fork failed.\n");
        exit(EXIT_FAILURE);
    }

    if (p > 0)
    {
        // Parent process writes to shared data
        printf("Número: ");
        scanf("%d", &shared_data->numero);

        printf("Nome: ");
        scanf(" %[^\n]", shared_data->nome);

        for (int i = 0; i < NR_DISC; i++)
        {
            printf("Nota da Cadeira %d:\n", i+1);
            scanf("%d", &shared_data->disciplinas[i]);
        }

        // Set flag to 1 to indicate shared data is available
        shared_data->flag = 1;

        // Wait for child process to complete
        wait(NULL);
    }
    else
    {
        // Child process reads from shared data
        while (shared_data->flag != 1)
        {
            ;
            // Wait for shared data to be available
        }

        int min = 999999, max = -11, sum = 0;
        double average = 0;
        for (int i = 0; i < NR_DISC; i++)
        {
            if (shared_data->disciplinas[i] < min)
            {
                min = shared_data->disciplinas[i];
            }
            if (shared_data->disciplinas[i] > max)
            {
                max = shared_data->disciplinas[i];
            }
            sum += shared_data->disciplinas[i];
        }

        pid_t p2;
        p2 = fork();
        if (p2 > 0)
        {
            printf("Print Filho 1\n");
            printf("Numero: %d\n", shared_data->numero);
            printf("Nome : %s\n", shared_data->nome);
            printf("Nota baixa: %d\n", min);
            printf("Nota  alta: %d\n", max);
        }
        else
        {
            printf("Print Filho 2 \n");
            average = ((double)sum) / NR_DISC;
            printf("Média de todas as notas: %.2f\n", average);
        }

        // Unmap shared memory area and close file descriptor
        if (munmap(shared_data, data_size) == -1)
        {
            perror("munmap error");
            exit(EXIT_FAILURE);
        }
    }
}
