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
    int fd, data_size = sizeof(Student);

    fd = shm_open("/ficheiro1", O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    ftruncate(fd, data_size);

    Student *shared_data = (Student *) mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    int i;
    for (i = 0; i < 10; i++)
    {
       printf("The number.%d -> %d.\n",i,shared_data->numbers[i]);
    }
    
    return EXIT_SUCCESS;
}