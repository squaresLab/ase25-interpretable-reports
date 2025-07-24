//GEMINI-pro DATASET v1.0 Category: Basic Unix-like Shell ; Style: calm
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

char **parseArgs(const char *line) {
    char *args[10];
    char *arg;
    char tmp[strlen(line)+1];
    strcpy(tmp, line);

    arg = strtok(tmp, " ");
    int i = 0;
    while (arg && i < 10) {
        args[i] = arg;
        i++;
        arg = strtok(NULL, " ");
    }
    args[i] = NULL;

    return args;
}

void execute(char **args) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        exit(1);
    } else {
        wait(NULL);
    }
}

int main() {
    char line[100];

    while (1) {
        printf("# ");
        fgets(line, sizeof(line), stdin);
        if (strlen(line) == 0) {
            continue;
        }
        if (strcmp(line, "exit\n") == 0) {
            break;
        }

        char **args = parseArgs(line);

        if (args == NULL) {
            continue;
        }

        execute(args);
    }

    return 0;
}