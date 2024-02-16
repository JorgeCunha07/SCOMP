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
    int numbers[10];
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
    while (student->sequencesNumber < 5)
    {
        if (student->canRead == 1)
        {
            int sum = 0;
            float average;

            for (i = 0; i < ARR_SIZE; i++)
            {
                sum += student->numbers[i];
            }
            average = sum / ARR_SIZE;
            printf("The number sequence number %d has an average of %.2f.\n", student->sequencesNumber, average);
            student->sequencesNumber++;
            student->canRead = 0;
            student->canWrite = 1;
        }
    }

    if (student, sizeof(Student) != 0)
    {
        perror("Munmap error");
        exit(1);
    }

    if (shm_unlink("/pl3ex04") != 0)
    {
        perror("Unlink error");
        exit(1);
    }

    return 0;
}
