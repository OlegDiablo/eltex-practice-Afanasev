#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <fcntl.h>
#include <unistd.h>

#define QUEUE_AB "/chat_ab"
#define QUEUE_BA "/chat_ba"

#define MSG_NORMAL 1
#define MSG_EXIT   9
#define MAX_SIZE 256

int main() {
    mqd_t mq_ab = mq_open(QUEUE_AB, O_RDONLY);
    mqd_t mq_ba = mq_open(QUEUE_BA, O_WRONLY);

    if (mq_ab == -1 || mq_ba == -1) {
        perror("mq_open");
        exit(1);
    }

    char buffer[MAX_SIZE];
    unsigned int prio;

    while (1) {
        mq_receive(mq_ab, buffer, MAX_SIZE, &prio);

        if (prio == MSG_EXIT) {
            mq_send(mq_ba, "exit", 5, MSG_EXIT);
            break;
        }

        printf("A > %s\n", buffer);

        printf("B > ");
        fgets(buffer, MAX_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        prio = (strcmp(buffer, "exit") == 0) ? MSG_EXIT : MSG_NORMAL;

        mq_send(mq_ba, buffer, strlen(buffer) + 1, prio);

        if (prio == MSG_EXIT)
            break;
    }

    mq_close(mq_ab);
    mq_close(mq_ba);

    return 0;
}