#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

#define MAX_SIZE 10
#define SHM_NAME "/my_shm"

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
    signal(SIGINT, sigint_handler);

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        exit(1);
    }

    if (ftruncate(shm_fd, sizeof(shared_data_t)) == -1) {
        perror("ftruncate failed");
        exit(1);
    }

    shared_data_t *data = mmap(NULL, sizeof(shared_data_t),
                               PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }

    data->ready = 0;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        while (!stop) {
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
            usleep(10000);
        }
        munmap(data, sizeof(shared_data_t));
        close(shm_fd);
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

        munmap(data, sizeof(shared_data_t));
        close(shm_fd);
        shm_unlink(SHM_NAME);
    }

    return 0;
}