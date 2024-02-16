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
    char address[100];
    char name[80];
    int number;

} Student;

int main() {
    int fd, data_size = sizeof(Student);
    
    fd = shm_open("/ficheiro1", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    ftruncate(fd, data_size);
    
    Student *shared_data = mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   
    printf("Address name:\n");
    scanf("%s", shared_data->address);
    
    printf("Student name:\n");
    scanf("%s", shared_data->name);

    printf("Student Number:\n");
    scanf("%d", &shared_data->number);



    return EXIT_SUCCESS;
} 