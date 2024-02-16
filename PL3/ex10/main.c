#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/wait.h>

#define SIZE 10

typedef int Vec[SIZE];

int main()
{
  int fd, data_size = sizeof(Vec);
  fd = shm_open("/pl3ex10", O_CREAT | O_TRUNC | O_RDWR, 0644);
  ftruncate(fd, data_size);
  int *shared_data = mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  srand(time(NULL));

  int parent[2];
  if (pipe(parent) == -1)
  {
    perror("Pipe failed");
    return 1;
  }

  int child[2];
  if (pipe(child) == -1)
  {
    perror("Pipe failed");
    return 1;
  }

  pid_t pid = fork();
  int i, j;
  int write_number = 1, read_number;

  if (pid == 0)
  {
    close(parent[1]);
    close(child[0]);

    while (read(parent[0], NULL, sizeof(int)) != 0)
    {
      sleep(1);
      printf("\n*In Child process*\n");

      for (i = 0; i < SIZE; i++)
      {
        printf("%d\n", shared_data[i]);
      }

      write(child[1], &write_number, sizeof(int));
      sleep(1);
    }

    close(parent[0]);
    close(child[1]);
  }

  close(parent[0]);
  close(child[1]);

  for (i = 0; i < 3; i++)
  {
    printf("\n*In Parent process*\n");

    for (j = 0; j < SIZE; j++)
    {
      shared_data[j] = rand();
      printf("%d\n", shared_data[j]);
    }

    write(parent[1], &write_number, sizeof(int));
    read(child[0], NULL, sizeof(int));
    sleep(1);
  }

  close(parent[1]);
  close(child[0]);
  wait(NULL);

  if (munmap(shared_data, data_size) != 0)
  {
    perror("Munmap error");
    exit(1);
  }

  if (close(fd) != 0)
  {
    perror("Pipe error");
    exit(1);
  }

  if (shm_unlink("/pl3ex10") != 0)
  {
    perror("Unlink error");
    exit(1);
  }

  return 0;
}