#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <netdb.h>

void dostuff(int);

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int nclients = 0;

void printusers() {
    if(nclients) {
        printf("%d user(s) on-line\n", nclients);
    }
    else {
        printf("No user on line\n");
    } 
}

int myfunc(int a, int b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : 0;
        default: return 0;
    }
}

void sigchld_handler(int s) {
    (void)s;
    while (waitpid(-1, NULL, WNOHANG) > 0)
        nclients--;
    printusers();
}

int main(int argc, char *argv[]) {
    int sockfd, newsockfd;
    int portno;
    int pid;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    printf("TCP SERVER DEMO\n");

    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }

    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    bzero((char*)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
        if (newsockfd < 0) error("ERROR on accept");

        nclients++;
        struct hostent *hst = gethostbyaddr((char*)&cli_addr.sin_addr, 4, AF_INET);
        printf("+%s [%s] new connect!\n",
               (hst) ? hst->h_name : "Unknown host",
               inet_ntoa(cli_addr.sin_addr));
        printusers();

        pid = fork();
        if (pid < 0) error("ERROR on fork");

        if (pid == 0) {
            close(sockfd);
            dostuff(newsockfd);
            exit(0);
        } else {
            close(newsockfd);
        }
    }

    close(sockfd);
    return 0;
}

void dostuff(int sock) {
    ssize_t bytes_recv;
    char buff[20 * 1024];
    char op;
    int a, b, result;

    #define MSG1 "Enter operation (+,-,*,/ or FILE for file transfer, quit to exit):\r\n"
    #define MSG2 "Enter first number:\r\n"
    #define MSG3 "Enter second number:\r\n"
    #define MSG4 "Result: "

    while (1) {
        write(sock, MSG1, strlen(MSG1));

        bytes_recv = read(sock, buff, sizeof(buff) - 1);
        if (bytes_recv <= 0) break; 
        buff[bytes_recv] = '\0';
        buff[strcspn(buff, "\r\n")] = 0; 

        if (strcmp(buff, "quit") == 0) {
            write(sock, "Goodbye\r\n", strlen("Goodbye\r\n"));
            break;
        }

        if (strncmp(buff, "FILE", 4) == 0) {
            while (1) {
                write(sock, "Send file name:\r\n", strlen("Send file name:\r\n"));
                bytes_recv = read(sock, buff, sizeof(buff) - 1);
                if (bytes_recv <= 0) break;
                buff[bytes_recv] = '\0';
                buff[strcspn(buff, "\r\n")] = 0;

                if (strlen(buff) == 0) {
                    write(sock, "Error: Filename cannot be empty\r\n",
                          strlen("Error: Filename cannot be empty\r\n"));
                    continue; 
                }

                FILE *f = fopen(buff, "wb");
                if (!f) {
                    write(sock, "Cannot create file\r\n", strlen("Cannot create file\r\n"));
                    continue;
                }

                write(sock, "Send file data, finish with EOF\r\n",
                      strlen("Send file data, finish with EOF\r\n"));

                while ((bytes_recv = read(sock, buff, sizeof(buff))) > 0) {
                    if (bytes_recv >= 3 &&
                        memcmp(buff + bytes_recv - 3, "EOF", 3) == 0) {
                        fwrite(buff, 1, bytes_recv - 3, f);
                        break;
                    }
                    fwrite(buff, 1, bytes_recv, f);
                }

                fclose(f);
                write(sock, "File received successfully\r\n",
                      strlen("File received successfully\r\n"));
                break; 
            }
            continue;
        }

        op = buff[0];
        if (op != '+' && op != '-' && op != '*' && op != '/') {
            write(sock, "Unknown operation. Use + - * / only\r\n",
                  strlen("Unknown operation. Use + - * / only\r\n"));
            continue;
        }

        while (1) {
            write(sock, MSG2, strlen(MSG2));
            bytes_recv = read(sock, buff, sizeof(buff) - 1);
            if (bytes_recv <= 0) break;
            buff[bytes_recv] = '\0';
            a = atoi(buff);
            break; 
        }

        while (1) {
            write(sock, MSG3, strlen(MSG3));
            bytes_recv = read(sock, buff, sizeof(buff) - 1);
            if (bytes_recv <= 0) break;
            buff[bytes_recv] = '\0';
            b = atoi(buff);
            break;
        }

        result = myfunc(a, b, op);
        snprintf(buff, sizeof(buff), "%s%d\r\n", MSG4, result);
        write(sock, buff, strlen(buff));
    }
}