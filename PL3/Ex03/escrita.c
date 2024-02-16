#include <fcntl.h> /* For constants O_* */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For constants “mode” */
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    int numbers[10];

} Student;

int main() {
    time_t t;
    srand((unsigned)time(&t));
    int fd, data_size = sizeof(Student);
    
    fd = shm_open("/ficheiro1", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    ftruncate(fd, data_size);
    
    Student *shared_data = mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   
   int i,valor = 0;
   for (i = 0; i < 10; i++){
    valor = rand() % 20 +1;
    printf("Number.%d: %d \n",i,valor);
    shared_data->numbers[i] = valor;
    sleep(1);
   }
   

    return EXIT_SUCCESS;
} 