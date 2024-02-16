#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define LOCAL_MAX_VALUES 10
#define ARR_SIZE 1000
#define CHILD_NUMBER 10

typedef struct
{
    int local_arr[LOCAL_MAX_VALUES];
} shared_data_type;

int main(void)
{
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;
    fd = shm_open("/pl3ex06", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    int global_arr[ARR_SIZE];
    int i, j;

    for (i = 0; i < ARR_SIZE; i++)
    {
        global_arr[i] = rand() % 1001;
    }

    pid_t pid[CHILD_NUMBER];

    for (i = 0; i < CHILD_NUMBER; i++)
    {
        pid[i] = fork();

        if (pid[i] == 0)
        {
            int local_max = 0;

            for (j = (100 * i); j < (100 * i) + 100; j++)
            {
                if (global_arr[j] > local_max)
                {
                    local_max = global_arr[j];
                }
            }

            shared_data->local_arr[i] = local_max;
            exit(0);
        }
    }

    wait(NULL);

    int global_max = 0;

    for (i = 0; i < LOCAL_MAX_VALUES; i++)
    {
        if (shared_data->local_arr[i] > global_max)
        {
            global_max = shared_data->local_arr[i];
        }
    }

    printf("The global maximum is: %d\n", global_max);

    if (munmap(shared_data, sizeof(shared_data_type)) != 0)
    {
        perror("Munmap error");
        exit(1);
    }

    if (shm_unlink("/pl3ex06") != 0)
    {
        perror("Unlink error");
        exit(1);
    }

    return 0;
}