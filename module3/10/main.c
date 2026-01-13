#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

#define MAX_SIZE 10  

typedef struct {
    int numbers[MAX_SIZE];
    int count;
    int min;
    int max;
    int ready; 
} shared_data_t;

volatile sig_atomic_t stop = 0;

void sigint_handler(int sig) {
    (void)sig;
    stop = 1;
}

int main() {
    key_t key = ftok("shmfile", 65); 
    int shmid = shmget(key, sizeof(shared_data_t), 0666|IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    shared_data_t *data = (shared_data_t*) shmat(shmid, NULL, 0);
    if (data == (void*) -1) {
        perror("shmat failed");
        exit(1);
    }

    signal(SIGINT, sigint_handler); 

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) { 
        
        while (1) {
            if (data->ready == 1) {
                int min = data->numbers[0];
                int max = data->numbers[0];
                for (int i = 1; i < data->count; i++) {
                    if (data->numbers[i] < min) min = data->numbers[i];
                    if (data->numbers[i] > max) max = data->numbers[i];
                }
                data->min = min;
                data->max = max;
                data->ready = 2;
            }
            if (stop) break; 
            usleep(10000);
        }
        shmdt(data);
        exit(0);
    } else {
        srand(time(NULL));
        int sets_processed = 0;

        while (!stop) {
            data->count = rand() % MAX_SIZE + 1;
            for (int i = 0; i < data->count; i++) {
                data->numbers[i] = rand() % 1000;
            }
            data->ready = 1; 

            while (data->ready != 2 && !stop) {
                usleep(10000);
            }

            if (stop) break;

            printf("Set %d: min=%d, max=%d\n", sets_processed+1, data->min, data->max);
            data->ready = 0;
            sets_processed++;
            sleep(1);
        }

        printf("\nTotal sets processed: %d\n", sets_processed);

        kill(pid, SIGINT);
        wait(NULL);
        shmdt(data);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}