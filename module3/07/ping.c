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
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    mqd_t mq_ab = mq_open(QUEUE_AB, O_CREAT | O_WRONLY, 0666, &attr);
    mqd_t mq_ba = mq_open(QUEUE_BA, O_CREAT | O_RDONLY, 0666, &attr);

    if (mq_ab == -1 || mq_ba == -1) {
        perror("mq_open");
        exit(1);
    }

    char buffer[MAX_SIZE];
    unsigned int prio;

    while (1) {
        printf("A > ");
        fgets(buffer, MAX_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        prio = (strcmp(buffer, "exit") == 0) ? MSG_EXIT : MSG_NORMAL;

        mq_send(mq_ab, buffer, strlen(buffer) + 1, prio);

        if (prio == MSG_EXIT)
            break;

        mq_receive(mq_ba, buffer, MAX_SIZE, &prio);

        if (prio == MSG_EXIT)
            break;

        printf("B > %s\n", buffer);
    }

    mq_close(mq_ab);
    mq_close(mq_ba);

    mq_unlink(QUEUE_AB);
    mq_unlink(QUEUE_BA);

    return 0;
}