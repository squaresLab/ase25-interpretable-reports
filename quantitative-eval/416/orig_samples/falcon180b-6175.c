//Falcon-180B DATASET v1.0 Category: System administration ; Style: innovative
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGUMENTS 10
#define MAX_ARGUMENT_LENGTH 128

typedef struct {
    char **arguments;
    int argument_count;
} Command;

int execute_command(Command *command) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        if (execvp(command->arguments[0], command->arguments) == -1) {
            perror("execvp");
            exit(1);
        }
    } else {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            fprintf(stderr, "Command terminated by signal %d\n", WTERMSIG(status));
            return 1;
        } else {
            fprintf(stderr, "Unknown exit status\n");
            return 1;
        }
    }
}

int parse_command(char *input, Command *command) {
    char *token;
    int argc = 0;
    char *argv[MAX_ARGUMENTS];

    token = strtok(input, " ");
    while (token!= NULL) {
        if (argc >= MAX_ARGUMENTS) {
            fprintf(stderr, "Too many arguments\n");
            return 1;
        }
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }
    command->arguments = argv;
    command->argument_count = argc;

    return 0;
}

int main() {
    char input[MAX_COMMAND_LENGTH];
    Command command;

    while (1) {
        printf("> ");
        fgets(input, MAX_COMMAND_LENGTH, stdin);
        if (parse_command(input, &command)!= 0) {
            continue;
        }
        execute_command(&command);
    }

    return 0;
}