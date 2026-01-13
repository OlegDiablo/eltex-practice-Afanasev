#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFER_SIZE 1024
#define SERVER_PORT 7777

char *SERVER_IP = "127.0.0.1";  

int sockfd;
struct sockaddr_in server_addr;

void *receive_messages(void *arg) {
    (void)arg;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);
    while (1) {
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                         (struct sockaddr *)&server_addr, &addr_len);
        if (n > 0) {
            buffer[n] = '\0';
            printf("\nMessage: %s\n> ", buffer);
            fflush(stdout);
        }
    }
    return NULL;
}

void *send_messages(void *arg) {
    (void)arg;
    char buffer[BUFFER_SIZE];
    while (1) {
        printf("> ");
        if (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0';
            sendto(sockfd, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&server_addr, sizeof(server_addr));
        }
    }
    return NULL;
}

int main() {
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creating error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    pthread_t recv_thread, send_thread;
    pthread_create(&recv_thread, NULL, receive_messages, NULL);
    pthread_create(&send_thread, NULL, send_messages, NULL);

    pthread_join(send_thread, NULL);
    close(sockfd);
    return 0;
}