#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define SERVER_TYPE 10
#define TEXT_SIZE 256

struct msgbuf {
    long mtype;
    long sender;
    char text[TEXT_SIZE];
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <client_priority>\n", argv[0]);
        exit(1);
    }

    long my_type = atol(argv[1]);

    key_t key = ftok("chatkey", 1);
    int msgid = msgget(key, 0666);

    if (msgid < 0) {
        perror("msgget");
        exit(1);
    }

    struct msgbuf msg;

    if (fork() == 0) {
        while (1) {
            if (msgrcv(msgid, &msg,
                       sizeof(msg) - sizeof(long),
                       my_type, 0) >= 0) {
                printf("From %ld: %s\n",
                       msg.sender, msg.text);
            }
        }
    }

    while (1) {
        msg.mtype = SERVER_TYPE;
        msg.sender = my_type;

        fgets(msg.text, TEXT_SIZE, stdin);
        msg.text[strcspn(msg.text, "\n")] = 0;

        msgsnd(msgid, &msg,
               sizeof(msg) - sizeof(long), 0);

    }

    return 0;
}