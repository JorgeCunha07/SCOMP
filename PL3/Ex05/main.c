#include <sys/mman.h> //shm_open, shm_unlink
#include <sys/stat.h> //Constants used for opening: S_IRUSR, S_IWUSR
#include <fcntl.h> //File Control: O_RDWR, O_CREAT, O_EXCL
#include <unistd.h> //ftruncate
#include <sys/types.h> //ftruncate
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct {
    int value;
} shared_data_type;

int main() {
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;

    shm_unlink("/ex05");
     //Removes the memory area from the file system

    fd = shm_open("/ex05", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR); 
    //Creates and opens a shared memory area
    ftruncate(fd, data_size); 
    //Defines the size
    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 
    //Maps the shared area in the process address space

    pid_t p;
    p = fork();

    if (p < 0) {
        perror("Fork failed.\n");
        exit(-1);
    }
    shared_data->value = 100;

    if (p == 0) {
        for (int i = 0; i < 1000000; i++) {
            shared_data->value++;
            shared_data->value--;
        }
        exit(0);
    }

    for (int i = 0; i < 1000000; i++) {
        shared_data->value++;
        shared_data->value--;
    }
    wait(NULL);
    printf("Value %d\n", shared_data->value);

    munmap(shared_data, data_size);
    close(fd);
}
