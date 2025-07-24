//FormAI DATASET v1.0 Category: File system simulation ; Style: introspective
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_FILE_SYSTEM_SIZE 1024*1024 //1MB filesystem
#define BLOCK_SIZE 1024 //1KB block size
#define MAX_FILES 20 //maximum number of files in file system

//structure of a file
typedef struct file {
    char *name;
    int start_block;
    int end_block;
    int size;
} file;

//structure of the file system
typedef struct file_system {
    char *disk[MAX_FILE_SYSTEM_SIZE / BLOCK_SIZE]; //file system disk
    int allocation_map[MAX_FILE_SYSTEM_SIZE / BLOCK_SIZE]; //1->allocated, 0->free
    file file_list[MAX_FILES]; //list of files in file system
} file_system;

//function to create a file system
void create_file_system(file_system *fs) {
    for(int i=0;i<MAX_FILE_SYSTEM_SIZE/BLOCK_SIZE;i++) {
        fs->disk[i] = NULL;
        fs->allocation_map[i] = 0; //initially all blocks are free
    }
    for(int i=0;i<MAX_FILES;i++) {
        fs->file_list[i].name = NULL;
        fs->file_list[i].start_block = -1; //initially no files exist
    }
}

//function to allocate a block for file
int allocate_block(file_system *fs) {
    int block = -1;
    for(int i=0;i<MAX_FILE_SYSTEM_SIZE/BLOCK_SIZE;i++) {
        if(fs->allocation_map[i] == 0) {
            block = i;
            fs->allocation_map[i] = 1;
            break;
        }
    }
    return block;
}

//function to create a file in file system
void create_file(file_system *fs, char *name, int size) {
    int blocks = size / BLOCK_SIZE;
    if(size % BLOCK_SIZE != 0)
        blocks++;
    int start_block = -1;
    for(int i=0;i<MAX_FILE_SYSTEM_SIZE/BLOCK_SIZE;i++) {
        int blocks_found = 0;
        if(fs->allocation_map[i] == 0) {
            for(int j=i;j<MAX_FILE_SYSTEM_SIZE/BLOCK_SIZE;j++) {
                if(fs->allocation_map[j] == 0) {
                    blocks_found++;
                    if(blocks_found == blocks) {
                        start_block = i;
                        break;
                    }
                }
                else {
                    blocks_found = 0;
                    i = j;
                    break;
                }
            }
        }
        if(start_block != -1)
            break;
    }
    if(start_block == -1)
        printf("ERROR: File could not be created. Not enough space in file system.\n");
    else {
        int i;
        for(i=0;i<MAX_FILES;i++) {
            if(fs->file_list[i].name == NULL) {
                fs->file_list[i].name = strdup(name);
                fs->file_list[i].start_block = start_block;
                fs->file_list[i].end_block = start_block + blocks - 1;
                fs->file_list[i].size = size;
                break;
            }
        }
        for(int j=start_block;j<=fs->file_list[i].end_block;j++) {
            fs->disk[j] = strdup(name);
            fs->allocation_map[j] = 1;
        }
        printf("SUCCESS: File %s created in file system with size %d.\n", name, size);
    }
}

//function to delete a file from file system
void delete_file(file_system *fs, char *name) {
    int i;
    for(i=0;i<MAX_FILES;i++) {
        if(fs->file_list[i].name != NULL && strcmp(fs->file_list[i].name, name) == 0) {
            for(int j=fs->file_list[i].start_block;j<=fs->file_list[i].end_block;j++) {
                fs->disk[j] = NULL;
                fs->allocation_map[j] = 0;
            }
            free(fs->file_list[i].name);
            fs->file_list[i].name = NULL;
            fs->file_list[i].start_block = -1;
            printf("SUCCESS: File %s deleted from file system.\n", name);
            break;
        }
    }
    if(i == MAX_FILES)
        printf("ERROR: File %s not found in file system.\n", name);
}

//function to display file system structure
void display_file_system(file_system *fs) {
    printf("FILE SYSTEM STRUCTURE:\n");
    printf("-----------------------\n");
    printf("File List:\n");
    printf("------------------------------------------------------------------------\n");
    printf("| %-10s | %-10s | %-10s | %-10s | %-10s |\n","File No.","Name","Start Block","End Block","Size");
    printf("------------------------------------------------------------------------\n");
    for(int i=0;i<MAX_FILES;i++) {
        if(fs->file_list[i].name != NULL) {
            printf("| %-10d | %-10s | %-10d | %-10d | %-10d |\n",i+1,fs->file_list[i].name,fs->file_list[i].start_block,fs->file_list[i].end_block,fs->file_list[i].size);
        }
    }
    printf("------------------------------------------------------------------------\n");
    printf("Disk Structure:\n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("| %-10s |","Block No.");
    for(int i=0;i<BLOCK_SIZE/10;i++)
        printf(" %-9d |",i*10);
    printf("\n--------------------------------------------------------------------------------------\n");
    for(int i=0;i<MAX_FILE_SYSTEM_SIZE/BLOCK_SIZE;i++) {
        if(i % (BLOCK_SIZE/10) == 0)
            printf("| %-10d |",i);
        if(fs->disk[i] == NULL)
            printf(" %-9s |","FREE");
        else
            printf(" %-9s |",fs->disk[i]);
        if((i+1) % (BLOCK_SIZE/10) == 0)
            printf("\n");
    }
    printf("--------------------------------------------------------------------------------------\n");
}

int main() {
    file_system fs;
    create_file_system(&fs);
    int choice;
    while(1) {
        printf("FILE SYSTEM SIMULATION\n");
        printf("1. Create File\n");
        printf("2. Delete File\n");
        printf("3. Display File System Structure\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d",&choice);
        switch(choice) {
            case 1:
                {
                    char name[20];
                    int size;
                    printf("Enter file name: ");
                    scanf("%s",name);
                    printf("Enter file size (in bytes): ");
                    scanf("%d",&size);
                    create_file(&fs,name,size);
                    break;
                }
            case 2:
                {
                    char name[20];
                    printf("Enter file name: ");
                    scanf("%s",name);
                    delete_file(&fs,name);
                    break;
                }
            case 3:
                {
                    display_file_system(&fs);
                    break;
                }
            case 4:
                {
                    printf("Exiting File System Simulation...\n");
                    exit(0);
                }
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}