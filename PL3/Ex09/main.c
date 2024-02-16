#include <sys/mman.h> //shm_open, shm_unlink
#include <sys/stat.h> //Constants used for opening: S_IRUSR, S_IWUSR
#include <fcntl.h> //File Control: O_RDWR, O_CREAT, O_EXCL
#include <unistd.h> //ftruncate
#include <sys/types.h> //ftruncate
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SIZE 10
#define OPERATIONS 3

typedef struct {
    int numbers[SIZE];
    int flag;
} shared_data_type;

int main() {
    int fd, data_size = sizeof(shared_data_type);
    shared_data_type *shared_data;

    shm_unlink("/Ex9"); 
    //Removes the memory area from the file system

    fd = shm_open("/ex9", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR); 
    //Creates and opens a shared memory area

    ftruncate(fd, data_size); 
    //Defines the size
   
    shared_data = (shared_data_type *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); 
    //Maps the shared area in the process address space

    shared_data->flag = 0;

    pid_t p;
    p = fork();

    if (p < 0) {
        perror("Fork failed.\n");
        exit(-1);
    }

    if (p > 0) {
        for (int i = 0; i < OPERATIONS; i++) {
            while (shared_data->flag != 1) { //espera que se torne 1
                ;
            }
            printf("Lista de comprador -> Interações %d:\n",i+1);
            for (int j = 0; j < SIZE; j++) {
                printf("valor %d -> %d\n",j+1, shared_data->numbers[j]);
            }
        }
        shared_data->flag = 0;
        wait(NULL);
    } 
    else {
        for (int i = 0; i < OPERATIONS; i++) {
            while (shared_data->flag == 1) {
                ;
            }
            int aux = 0,valorCheck;
            printf("Vendedor lista -> %d\n",i);
            for (int i = 0; i < SIZE; i++) {
                if (i!=0)
                {
                    aux = shared_data->numbers[i-1];
                }

                valorCheck = rand() % 100001;

                while (aux > valorCheck)
                {
                    valorCheck = rand() % 100001;
                }
                
                shared_data->numbers[i] = valorCheck;
            }
        }
        shared_data->flag = 1;
        exit(0);
    }

        if (munmap(shared_data, data_size) == -1)
        {
            perror("munmap error");
            exit(EXIT_FAILURE);
        }
        
    close(fd);
}