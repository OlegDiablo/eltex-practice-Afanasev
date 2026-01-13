#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define BUFFER_SIZE 65536
#define SERVER_PORT 7777

int main() {
    int sockfd;
    unsigned char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    printf("RAW UDP-sniffer is running\n");

    while (1) {
        ssize_t size = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (size < 0) {
            perror("recv");
            continue;
        }

        struct iphdr *ip = (struct iphdr *)buffer;
        if (ip->protocol != IPPROTO_UDP)
            continue;

        struct udphdr *udp = (struct udphdr *)(buffer + ip->ihl * 4);

        if (ntohs(udp->dest) != SERVER_PORT)
            continue;

        unsigned char *payload = buffer + ip->ihl * 4 + sizeof(struct udphdr);

        int payload_len = size - (ip->ihl * 4 + sizeof(struct udphdr));

        printf("Message %s:%d → server:\n",
               inet_ntoa(*(struct in_addr *)&ip->saddr),
               ntohs(udp->source));

        fwrite(payload, 1, payload_len, stdout);
        printf("\n-----------------------------\n");
    }

    close(sockfd);
    return 0;
}