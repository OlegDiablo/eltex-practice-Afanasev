#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <time.h>
#include <string.h>

#define MAX_NUMBERS 10
#define MAX_LINE 1024

int main() {
    const char *filename = "data.txt";
    const char *sem_name = "/file_semaphore";

    sem_unlink(sem_name);

    sem_t *sem = sem_open(sem_name, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        while (1) {
            sem_wait(sem);

            FILE *f = fopen(filename, "r+");
            if (!f) {
                perror("fopen");
                sem_post(sem);
                exit(1);
            }

            char line[MAX_LINE];
            while (fgets(line, MAX_LINE, f)) {
                int min = 101, max = -1, num;
                char *token = strtok(line, " \n");
                while (token) {
                    num = atoi(token);
                    if (num < min) min = num;
                    if (num > max) max = num;
                    token = strtok(NULL, " \n");
                }
                printf("Min: %d, Max: %d\n", min, max);
            }

            freopen(filename, "w", f);
            fclose(f);

            sem_post(sem);
            sleep(1);
        }
    } else {
        srand(time(NULL));
        while (1) {
            int n = rand() % MAX_NUMBERS + 1;

            sem_wait(sem);

            FILE *f = fopen(filename, "a");
            if (!f) {
                perror("fopen");
                sem_post(sem);
                exit(1);
            }

            for (int i = 0; i < n; i++) {
                fprintf(f, "%d ", rand() % 100);
            }
            fprintf(f, "\n");
            fclose(f);

            sem_post(sem);
            sleep(1);
        }

        wait(NULL);
    }

    sem_close(sem);
    sem_unlink(sem_name);

    return 0;
}