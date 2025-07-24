//Falcon-180B DATASET v1.0 Category: File Backup System ; Style: asynchronous
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>

#define MAX_BACKUP_FILES 10

struct backup_file {
    char filename[100];
    time_t timestamp;
};

struct backup_file backup_files[MAX_BACKUP_FILES];
int num_backup_files = 0;

void add_backup_file(char *filename) {
    if (num_backup_files >= MAX_BACKUP_FILES) {
        printf("Error: Backup file limit reached.\n");
        return;
    }
    strcpy(backup_files[num_backup_files].filename, filename);
    backup_files[num_backup_files].timestamp = time(NULL);
    num_backup_files++;
}

void print_backup_files() {
    printf("Backup files:\n");
    for (int i = 0; i < num_backup_files; i++) {
        printf("%s (%ld)\n", backup_files[i].filename, backup_files[i].timestamp);
    }
}

void *backup_thread(void *arg) {
    char source_file[100];
    char dest_file[200];
    strcpy(source_file, (char *)arg);
    strcat(dest_file, "backup_");
    strcat(dest_file, source_file);

    FILE *source = fopen(source_file, "rb");
    if (source == NULL) {
        printf("Error: Could not open source file %s.\n", source_file);
        return NULL;
    }

    FILE *dest = fopen(dest_file, "wb");
    if (dest == NULL) {
        printf("Error: Could not create destination file %s.\n", dest_file);
        fclose(source);
        return NULL;
    }

    char buffer[1024];
    while (fread(buffer, 1, 1024, source) > 0) {
        fwrite(buffer, 1, 1024, dest);
    }

    fclose(source);
    fclose(dest);

    add_backup_file(dest_file);

    printf("Backup of %s complete.\n", source_file);
    return NULL;
}

int main() {
    pthread_t thread_id;
    char source_file[100];

    printf("Enter source file name: ");
    scanf("%s", source_file);

    pthread_create(&thread_id, NULL, backup_thread, (void *)source_file);

    printf("Press any key to exit...\n");
    getchar();

    return 0;
}