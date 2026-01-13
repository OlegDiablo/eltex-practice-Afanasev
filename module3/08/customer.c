#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#define MAX_LINE 1024

void sem_lock(int semid) {
    struct sembuf op = {0, -1, 0};
    semop(semid, &op, 1);
}

void sem_unlock(int semid) {
    struct sembuf op = {0, 1, 0};
    semop(semid, &op, 1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    key_t key = ftok(filename, 'A');
    int semid = semget(key, 1, 0666);

    while (1) {
        sem_lock(semid);

        FILE *f = fopen(filename, "r+");
        if (!f) {
            perror("fopen");
            sem_unlock(semid);
            exit(1);
        }

        char line[MAX_LINE];
        while (fgets(line, MAX_LINE, f)) {
            int min = 101, max = -1;
            int num;
            char *token = strtok(line, " \n");
            while (token) {
                num = atoi(token);
                if (num < min) min = num;
                if (num > max) max = num;
                token = strtok(NULL, " \n");
            }
            printf("Min: %d, Max: %d\n", min, max);
        }

        // Очистка файла после обработки
        freopen(filename, "w", f);
        fclose(f);

        sem_unlock(semid);

        sleep(1);
    }

    return 0;
}