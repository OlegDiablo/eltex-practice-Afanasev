#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#define MAX_NUMBERS 10

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
    int semid = semget(key, 1, IPC_CREAT | 0666);
    semctl(semid, 0, SETVAL, 1); 

    srand(time(NULL));

    while (1) {
        int n = rand() % MAX_NUMBERS + 1; 

        sem_lock(semid);

        FILE *f = fopen(filename, "a");
        if (!f) {
            perror("fopen");
            sem_unlock(semid);
            exit(1);
        }

        for (int i = 0; i < n; i++) {
            fprintf(f, "%d ", rand() % 100);
        }
        fprintf(f, "\n");
        fclose(f);

        sem_unlock(semid);

        sleep(1); 
    }

    return 0;
}