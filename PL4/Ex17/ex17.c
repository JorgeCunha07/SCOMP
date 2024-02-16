#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

#define TOTAL_SS 5
#define TOTAL_MTS 6

typedef struct {
    int measureArray[TOTAL_SS];
    int alarmTotal;
} sensors;

void generate_random_measurements(sensors *s_info, int sensor_index, sem_t *semaforo_memory, sem_t *semaforo_info, sem_t *semaforo_read) {
    int randomNumber, alarmState = 0;
    int topMeasures = TOTAL_MTS * TOTAL_SS;

    srand(time(NULL));

    for (int j = 0; j < TOTAL_MTS; j++) {
        randomNumber = rand() % 100;

        sem_wait(semaforo_memory);

        if (alarmState == 0 && randomNumber > 50) {
            alarmState = 1;
            s_info->alarmTotal += 1;
            

            sem_post(semaforo_info);
            sem_wait(semaforo_read);
        } else if (randomNumber < 50 && s_info->measureArray[sensor_index] < 50 && alarmState == 1 ) {
            s_info->alarmTotal -= 1;
            alarmState = 0;
            
            sem_post(semaforo_info);
            sem_wait(semaforo_read);
        }

        s_info->measureArray[sensor_index] = randomNumber;

        sem_post(semaforo_info);
        sem_wait(semaforo_read);

        sem_post(semaforo_memory);

        sleep(1);
    }

    exit(0);
}

int main() {

    pid_t p;
    int shared;
    int data_size = sizeof(sensors);
    int aux = 0;
    sem_t *semaforo_memory, *semaforo_info, *semaforo_read;

    // Create shared memory to hold sensor data
    shared = shm_open("/sharedmemory17", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
    ftruncate(shared, data_size);
    sensors *s_info = (sensors *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, shared, 0);

    s_info->alarmTotal = 0;

    if ((semaforo_memory = sem_open("semaforo_memory", O_CREAT, S_IRUSR | S_IWUSR, 1)) == SEM_FAILED) {
        perror("Error.\n");
        exit(1);
    }

    if ((semaforo_info = sem_open("semaforo_info", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED) {
        perror("Error.\n");
        exit(1);
    }

    if ((semaforo_read = sem_open("semaforo_read", O_CREAT, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED) {
        perror("Error.\n");
        exit(1);
    }

    for (int i = 0; i < TOTAL_SS; i++) {
        p = fork();

        if (p == 0) {
             printf("Child: \n");
            generate_random_measurements(s_info, i, semaforo_memory, semaforo_info, semaforo_read);
        }
    }

    if (p > 0) {
        
        int topMeasures = TOTAL_MTS * TOTAL_SS;
        int mReceived = 0;

        while (mReceived != topMeasures) {
            sem_wait(semaforo_info);

            if (aux != s_info->alarmTotal) {
                printf("Total em alarme -> %d\n", s_info->alarmTotal);
                aux = s_info->alarmTotal;

                sem_post(semaforo_read);
                sem_wait(semaforo_info);
            }

            for (int k = 0; k < TOTAL_SS; k++) {
                printf("Sensor -> %d Measurements -> %d\n", k + 1, s_info->measureArray[k]);
            }
            sem_post(semaforo_read);
            
            mReceived++;
        }

        for (int x = 0; x < TOTAL_SS; x++) {
            wait(NULL);// waiting for the childs.
        }

        
         if (sem_unlink("semaforo_memory") == -1) {
            perror("Error.\n");//delete signal
            exit(1);
        }
        if (shm_unlink("sharedmemory17") == -1) {
            perror("Error.\n");//delete shared memory
            exit(1);
        }
        if (sem_unlink("semaforo_info") == -1) {
            perror("Error.\n");//delete signal
            exit(1);
        }

        if (sem_unlink("semaforo_read") == -1) {
            perror("Error.\n");//delete signal
            exit(1);
        }

        
        if (munmap(s_info, data_size) == -1) {
            perror("Error.\n");// close
            exit(1);
        }

         if (close(shared) == -1) {
            perror("Error.\n");// close
            exit(1);
        }

        exit(0);
    }
}
