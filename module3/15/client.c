#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int my_sock, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buff[1024];

    printf("TCP DEMO CLIENT\n");

    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);

    my_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (my_sock < 0) error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    // Соединение с сервером
    if (connect(my_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    while (1) {

        n = recv(my_sock, buff, sizeof(buff) - 1, 0);
        if (n <= 0) {
            printf("Server disconnected or error\n");
            break;
        }
        buff[n] = '\0';
        printf("%s", buff);

        printf("> ");
        fgets(buff, sizeof(buff) - 1, stdin);

        if (strncmp(buff, "quit", 4) == 0) {
            printf("Exit...\n");
            close(my_sock);
            break;
        }

        if (strncmp(buff, "FILE", 4) == 0) {

            send(my_sock, buff, strlen(buff), 0);

            char filename[256];
            printf("Enter filename: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;

            send(my_sock, filename, strlen(filename), 0);

            FILE *f = fopen(filename, "rb");
            if (!f) {
                printf("Cannot open file\n");
                continue;
            }

            size_t bytes;
            while ((bytes = fread(buff, 1, sizeof(buff), f)) > 0) {
                send(my_sock, buff, bytes, 0);
            }

            send(my_sock, "EOF", 3, 0);

            fclose(f);
            printf("File sent successfully\n");
            continue;
        }

        send(my_sock, buff, strlen(buff), 0);
    }

    close(my_sock);
    return 0;
}