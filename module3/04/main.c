#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_ARGS 64

int parse_args(char *command, char *args[], char **input_file, char **output_file) {
    int argc = 0;
    *input_file = NULL;
    *output_file = NULL;

    char *token = strtok(command, " \t\n");
    while (token != NULL && argc < MAX_ARGS - 1) {
        if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " \t\n");
            *input_file = token;
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " \t\n");
            *output_file = token;
        } else {
            args[argc++] = token;
        }
        token = strtok(NULL, " \t\n");
    }
    args[argc] = NULL;
    return argc;
}

void execute_single_command(char *args[], char *input_file, char *output_file, int input_fd, int output_fd) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork error");
        return;
    }

    if (pid == 0) {
        if (input_file != NULL) {
            int fd = open(input_file, O_RDONLY);
            if (fd < 0) {
                perror("Error with opening file for reading");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        } else if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
        }

        if (output_file != NULL) {
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("Error with opening file for writing");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } else if (output_fd != STDOUT_FILENO) {
            dup2(output_fd, STDOUT_FILENO);
        }

        execvp(args[0], args);
        if (errno == ENOENT) {
            printf("Command not found: %s\n", args[0]);
        } else {
            perror("Runtime error");
        }
        exit(1);
    }
}

void execute_pipeline(char *input) {
    char *cmd = NULL;
    int prev_fd = STDIN_FILENO;
    int pipe_fd[2];

    char *saveptr;
    cmd = strtok_r(input, "|", &saveptr);

    while (cmd != NULL) {
        char *args[MAX_ARGS];
        char *input_file = NULL;
        char *output_file = NULL;
        parse_args(cmd, args, &input_file, &output_file);

        char *next_cmd = strtok_r(NULL, "|", &saveptr);

        if (next_cmd != NULL) {
            pipe(pipe_fd);
            execute_single_command(args, input_file, NULL, prev_fd, pipe_fd[1]);
            close(pipe_fd[1]);
            if (prev_fd != STDIN_FILENO) close(prev_fd);
            prev_fd = pipe_fd[0];
        } else {
            execute_single_command(args, input_file, output_file, prev_fd, STDOUT_FILENO);
            if (prev_fd != STDIN_FILENO) close(prev_fd);
        }

        cmd = next_cmd;
    }

    while (wait(NULL) > 0);
}

int main() {
    char input[MAX_INPUT_LENGTH];

    printf("Extended command interpreter\n");
    printf("Type 'exit' to trrminate the program\n\n");

    while (1) {
        printf("myshell> ");
        fflush(stdout);

        if (fgets(input, MAX_INPUT_LENGTH, stdin) == NULL) {
            printf("\nGoodbye!\n");
            break;
        }

        if (strncmp(input, "exit", 4) == 0) {
            printf("Goodbye!\n");
            break;
        }

        if (strlen(input) <= 1) continue;

        execute_pipeline(input);
    }

    return 0;
}