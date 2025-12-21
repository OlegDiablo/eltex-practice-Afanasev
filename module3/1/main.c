#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>

int isNumber(char* arg) {

    if(arg == NULL || *arg == '\0') {
        return 0;
    }

    char* endptr;
    errno = 0;
    strtod(arg, &endptr);


    if(*endptr == '\0' && endptr != arg && errno != ERANGE) {
        return 1;
    }

    return 0;

}


void procArgs(char* argv[], int start, int end, char* proc) {

    for(int i = start; i < end; i++) {
        if(isNumber(argv[i]) == 1) {
            double num = atof(argv[i]) * 2;
            printf("%s process: %s * 2 = %.2f\n", proc, argv[i], num);
        }
        else {
            printf("%s process: %s\n", proc, argv[i]);
        }
    }

}


int main(int argc, char* argv[]) {

    int mid = (argc + 1) / 2;

    pid_t pid = fork();

    if(pid == -1) {
        perror("Proccess creating error");
    }
    
    if(pid == 0) {
        procArgs(argv, 1, mid, "child");
    }
    else {
        procArgs(argv, mid, argc, "parent");
        wait(NULL);
    }
    return 0;
}