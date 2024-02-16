#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>

#define ARR_SIZE 10

typedef struct
{
    int numbers[ARR_SIZE];
    int canRead;
    int canWrite;
    int sequencesNumber;
} Student;

int main()
{
    int fd, data_size = sizeof(Student);
    Student *student;
    fd = shm_open("/pl3ex04", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(fd, data_size);
    student = (Student *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    int i;

    student->canRead = 0;
    student->canWrite = 1;
    student->sequencesNumber = 0;

    while (student->sequencesNumber < 5)
    {
        if (student->canWrite == 1)
        {
            for (i = 0; i < ARR_SIZE; i++)
            {
                student->numbers[i] = rand() % 5 + 1;
            }
            student->canRead = 1;
            student->canWrite = 0;
        }
    }

    return 0;
}
