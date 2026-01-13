#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 20480

int nclients = 0;

/* Функция вычислений */
int myfunc(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return (b != 0) ? a / b : 0;
        default: return 0;
    }
}

void printusers() {
    if (nclients)
        printf("%d user(s) online\n", nclients);
    else
        printf("No users online\n");
}

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int listen_sock, new_sock;
    int client_socks[MAX_CLIENTS] = {0};
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char buff[BUFFER_SIZE];
    ssize_t bytes_recv;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(1);
    }

    int portno = atoi(argv[1]);

    // Создание слушающего сокета
    if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        error("socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(listen_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("bind");

    if (listen(listen_sock, 5) < 0)
        error("listen");

    printf("Server listening on port %d\n", portno);

    fd_set readfds;
    int max_fd;

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(listen_sock, &readfds);
        max_fd = listen_sock;

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_socks[i] > 0) {
                FD_SET(client_socks[i], &readfds);
                if (client_socks[i] > max_fd)
                    max_fd = client_socks[i];
            }
        }

        if (select(max_fd + 1, &readfds, NULL, NULL, NULL) < 0)
            error("select");

        if (FD_ISSET(listen_sock, &readfds)) {
            clilen = sizeof(cli_addr);
            if ((new_sock = accept(listen_sock, (struct sockaddr *)&cli_addr, &clilen)) < 0)
                error("accept");

            printf("New connection from %s\n", inet_ntoa(cli_addr.sin_addr));

            int added = 0;
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_socks[i] == 0) {
                    client_socks[i] = new_sock;
                    added = 1;
                    break;
                }
            }

            if (!added) {
                write(new_sock, "Server full\n", 12);
                close(new_sock);
            } else {
                nclients++;
                printusers();
                write(new_sock, "Enter operation (+,-,*,/ or FILE for file transfer, quit to exit):\r\n", 
                strlen("Enter operation (+,-,*,/ or FILE for file transfer, quit to exit):\r\n"));
                
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            int sock = client_socks[i];
            if (sock > 0 && FD_ISSET(sock, &readfds)) {
                bytes_recv = read(sock, buff, sizeof(buff) - 1);
                if (bytes_recv <= 0) {
                    printf("Client disconnected\n");
                    close(sock);
                    client_socks[i] = 0;
                    nclients--;
                    printusers();
                    continue;
                }

                buff[bytes_recv] = '\0';

                if (strncmp(buff, "quit", 4) == 0) {
                    write(sock, "Goodbye\n", 8);
                    close(sock);
                    client_socks[i] = 0;
                    nclients--;
                    printusers();
                    continue;
                }
                if (strncmp(buff, "FILE", 4) == 0) {
                    write(sock, "Send filename:\n", strlen("Send filename:\n"));
                    bytes_recv = read(sock, buff, sizeof(buff) - 1);
                    if (bytes_recv <= 0) continue;
                    buff[bytes_recv] = '\0';
                    buff[strcspn(buff, "\n")] = 0;

                    FILE *f = fopen(buff, "wb");
                    if (!f) {
                        write(sock, "Cannot create file\n", strlen("Cannot create file\n"));
                        continue;
                    }

                    write(sock, "Send file data, finish with EOF\n",
                          strlen("Send file data, finish with EOF\n"));

                    while ((bytes_recv = read(sock, buff, sizeof(buff))) > 0) {
                        if (bytes_recv >= 3 &&
                            memcmp(buff + bytes_recv - 3, "EOF", 3) == 0) {
                            fwrite(buff, 1, bytes_recv - 3, f);
                            break;
                        }
                        fwrite(buff, 1, bytes_recv, f);
                    }

                    fclose(f);
                    write(sock, "File received successfully\n",
                          strlen("File received successfully\n"));
                    continue;
                }


                char op = buff[0];
                int a, b, result;
                if (op != '+' && op != '-' && op != '*' && op != '/') {
                    write(sock, "Unknown operation. Use + - * / only.\n",
                        strlen("Unknown operation. Use + - * / only.\n"));
                    continue;
                }

                write(sock, "Enter first number:\n", strlen("Enter first number:\n"));
                bytes_recv = read(sock, buff, sizeof(buff) - 1);
                if (bytes_recv <= 0) continue;
                buff[bytes_recv] = '\0';
                a = atoi(buff);

                write(sock, "Enter second number:\n", strlen("Enter second number:\n"));
                bytes_recv = read(sock, buff, sizeof(buff) - 1);
                if (bytes_recv <= 0) continue;
                buff[bytes_recv] = '\0';
                b = atoi(buff);

                result = myfunc(a, b, op);

                snprintf(buff, sizeof(buff), "Result = %d\n", result);
                write(sock, buff, strlen(buff));
            }
        }
    }

    close(listen_sock);
    return 0;
}