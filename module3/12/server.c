#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 2
#define SERVER_PORT 7777


int main() {
    int sockfd;
    struct sockaddr_in server_addr, clients[MAX_CLIENTS];
    socklen_t addr_len = sizeof(struct sockaddr_in);
    int client_count = 0;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creating error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding error");
        exit(1);
    }

    char buffer[BUFFER_SIZE];
    printf("Server is running. Waiting for clients\n");

    while (1) {
        struct sockaddr_in sender_addr;
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                         (struct sockaddr *)&sender_addr, &addr_len);
        if (n < 0) continue;

        buffer[n] = '\0';

        int known = 0;
        for (int i = 0; i < client_count; i++) {
            if (clients[i].sin_addr.s_addr == sender_addr.sin_addr.s_addr &&
                clients[i].sin_port == sender_addr.sin_port) {
                known = 1;
                break;
            }
        }
        if (!known && client_count < MAX_CLIENTS) {
            clients[client_count++] = sender_addr;
            printf("Client %s:%d connected\n",
                   inet_ntoa(sender_addr.sin_addr), ntohs(sender_addr.sin_port));
        }


        for (int i = 0; i < client_count; i++) {
            if (clients[i].sin_addr.s_addr != sender_addr.sin_addr.s_addr ||
                clients[i].sin_port != sender_addr.sin_port) {
                sendto(sockfd, buffer, n, 0, (struct sockaddr *)&clients[i], addr_len);
            }
        }
    }

    close(sockfd);
    return 0;
}