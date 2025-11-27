#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 64

int parse_input(char *input, char *args[]) {
    int argc = 0;
    char *token = strtok(input, " \t\n");
    
    while (token != NULL && argc < MAX_ARGS - 1) {
        args[argc++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[argc] = NULL;
    
    return argc;
}


void execute_command(char *args[]) {
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("Ошибка при создании процесса");
        return;
    }
    
    if (pid == 0) {

        execvp(args[0], args);
        

        if (errno == ENOENT) {
            printf("Команда не найдена: %s\n", args[0]);
        } else {
            perror("Ошибка выполнения");
        }
        exit(1);
    } else {

        int status;
        waitpid(pid, &status, 0);
    }
}

int main() {
    char input[MAX_INPUT_LENGTH];
    char *args[MAX_ARGS];
    
    printf("=== Простой командный интерпретатор ===\n");
    printf("Введите 'exit' для выхода\n\n");
    
    while (1) {

        printf("myshell> ");
        fflush(stdout);
        

        if (fgets(input, MAX_INPUT_LENGTH, stdin) == NULL) {
            printf("\nДо свидания!\n");
            break;
        }

        if (strncmp(input, "exit", 4) == 0) {
            printf("До свидания!\n");
            break;
        }
        
        if (strlen(input) <= 1) {
            continue;
        }
        
        int argc = parse_input(input, args);
        
        if (argc > 0) {
            execute_command(args);
        }
    }
    
    return 0;
}