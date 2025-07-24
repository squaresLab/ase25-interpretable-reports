//Falcon-180B DATASET v1.0 Category: RAM usage monitor ; Style: scientific
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 1024
#define MAX_PROCESS_NAME_LENGTH 64

typedef struct {
    int pid;
    char name[MAX_PROCESS_NAME_LENGTH];
    int ram_usage;
} process_t;

int get_process_info(int pid, char *name, int *ram_usage) {
    FILE *fp;
    char path[64];
    char line[1024];
    int ret;
    
    sprintf(path, "/proc/%d/statm", pid);
    
    fp = fopen(path, "r");
    if (fp == NULL) {
        return -1;
    }

    fgets(line, sizeof(line), fp);
    ret = sscanf(line, "%*d %*d %*d %*d %*d %d", ram_usage);

    if (ret!= 1) {
        fclose(fp);
        return -1;
    }

    fclose(fp);
    strncpy(name, (char *)get_process_name(pid), MAX_PROCESS_NAME_LENGTH);

    return 0;
}

int get_process_name(int pid) {
    char path[64];
    FILE *fp;
    char line[1024];
    
    sprintf(path, "/proc/%d/cmdline", pid);

    fp = fopen(path, "r");
    if (fp == NULL) {
        return -1;
    }

    fgets(line, sizeof(line), fp);
    strtok(line, "\0");
    fclose(fp);

    return (int)line;
}

int main() {
    int i, num_processes, ret;
    process_t *processes;

    num_processes = 0;
    ret = 0;

    processes = (process_t *)malloc(MAX_PROCESSES * sizeof(process_t));
    if (processes == NULL) {
        printf("Error: failed to allocate memory for process list.\n");
        return -1;
    }

    for (i = 0; i < MAX_PROCESSES; i++) {
        ret = get_process_info(i, processes[i].name, &processes[i].ram_usage);
        if (ret == 0) {
            num_processes++;
        }
    }

    printf("Number of processes: %d\n", num_processes);

    for (i = 0; i < num_processes; i++) {
        printf("Process %d: %s\n", processes[i].pid, processes[i].name);
        printf("RAM usage: %d KB\n", processes[i].ram_usage);
        printf("\n");
    }

    free(processes);

    return 0;
}