//Falcon-180B DATASET v1.0 Category: Basic Unix-like Shell ; Style: scientific
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGUMENTS 256

// Function prototypes
int parse_command(char* command, char*** arguments);
int execute_command(char** arguments);
void print_prompt();

int main() {
    char command[MAX_COMMAND_LENGTH];
    char* arguments[MAX_ARGUMENTS];
    int status;

    print_prompt();

    while (fgets(command, MAX_COMMAND_LENGTH, stdin)!= NULL) {
        status = parse_command(command, &arguments);

        if (status == 0) {
            execute_command(arguments);
        } else {
            printf("Invalid command.\n");
        }

        print_prompt();
    }

    return 0;
}

int parse_command(char* command, char*** arguments) {
    int argc = 0;
    char* argv[MAX_ARGUMENTS];
    char* token;

    token = strtok(command, " ");
    while (token!= NULL) {
        argv[argc++] = token;
        if (argc >= MAX_ARGUMENTS) {
            printf("Too many arguments.\n");
            return 1;
        }
        token = strtok(NULL, " ");
    }

    if (argc == 0) {
        printf("Invalid command.\n");
        return 1;
    }

    *arguments = argv;
    return 0;
}

int execute_command(char** arguments) {
    pid_t pid;
    int status;

    if (strcmp(arguments[0], "exit") == 0) {
        return 0;
    } else if (strcmp(arguments[0], "cd") == 0) {
        if (arguments[1] == NULL) {
            printf("Usage: cd [directory]\n");
        } else {
            if (chdir(arguments[1])!= 0) {
                perror("chdir");
            }
        }
    } else {
        pid = fork();

        if (pid == 0) {
            if (execvp(arguments[0], arguments) == -1) {
                perror("execvp");
            }
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("fork");
        } else {
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}

void print_prompt() {
    printf("> ");
}