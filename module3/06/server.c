#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define SERVER_TYPE 10
#define MAX_CLIENTS 32
#define TEXT_SIZE 256

struct msgbuf {
    long mtype;
    long sender;
    char text[TEXT_SIZE];
};

int main() {
    key_t key = ftok("chatkey", 1);
    int msgid = msgget(key, IPC_CREAT | 0666);

    if (msgid < 0) {
        perror("msgget");
        exit(1);
    }

    long clients[MAX_CLIENTS];
    int client_count = 0;

    struct msgbuf msg;

    printf("Server started\n");

    while (1) {
        if (msgrcv(msgid, &msg, sizeof(msg) - sizeof(long),
                   SERVER_TYPE, 0) < 0) {
            perror("msgrcv");
            continue;
        }

        int known = 0;
        for (int i = 0; i < client_count; i++) {
            if (clients[i] == msg.sender) {
                known = 1;
                break;
            }
        }

        if (!known && client_count < MAX_CLIENTS) {
            clients[client_count++] = msg.sender;
            printf("Client %ld connected\n", msg.sender);
        }

        if (strcmp(msg.text, "shutdown") == 0) {
            printf("Client %ld disconnected\n", msg.sender);
            for (int i = 0; i < client_count; i++) {
                if (clients[i] == msg.sender) {
                    clients[i] = clients[--client_count];
                    break;
                }
            }
            continue;
        }

        printf("Message from client %ld: %s\n", msg.sender, msg.text);

        for (int i = 0; i < client_count; i++) {
            if (clients[i] != msg.sender) {
                struct msgbuf out;
                out.mtype = clients[i];
                out.sender = msg.sender;
                strncpy(out.text, msg.text, TEXT_SIZE);

                msgsnd(msgid, &out,
                       sizeof(out) - sizeof(long), 0);
            }
        }
    }
}