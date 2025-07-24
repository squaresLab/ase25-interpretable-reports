//FormAI DATASET v1.0 Category: File system simulation ; Style: authentic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_SIZE 1024 // Maximum size of a file in the file system
#define NUM_FILES 10 // Number of files in the file system
#define NUM_BLOCKS 20 // Number of blocks in the file system
#define BLOCK_SIZE 64 // Size of each block in bytes

typedef struct {
    char name[20]; // Name of file
    int size; // Size of file in bytes
    int start_block; // Index of first block in file
} file_info;

typedef struct {
    int size; // Size of file system in bytes
    int free_space; // Amount of free space in file system in bytes
    char* data[NUM_BLOCKS]; // Array of data blocks
    file_info files[NUM_FILES]; // Array of file information
} file_system;

// Initializes the file system with the given size and number of files
void init_file_system(file_system* fs, int size, int num_files) {
    fs->size = size;
    fs->free_space = size - (num_files * sizeof(file_info));
    for (int i = 0; i < NUM_BLOCKS; i++) {
        fs->data[i] = (char*)malloc(BLOCK_SIZE);
    }
    for (int i = 0; i < num_files; i++) {
        strcpy(fs->files[i].name, "");
        fs->files[i].size = 0;
        fs->files[i].start_block = -1;
    }
}

// Writes the given data to the given block of the file system
void write_block(file_system* fs, int block_num, char* data) {
    if (strlen(data) > BLOCK_SIZE) {
        printf("Error: Data too large for block size\n");
        return;
    }
    strcpy(fs->data[block_num], data);
}

// Reads data from the given block of the file system into the given buffer
void read_block(file_system* fs, int block_num, char* buffer) {
    strcpy(buffer, fs->data[block_num]);
}

// Adds a new file to the file system with the given name and size
void add_file(file_system* fs, char* name, int size) {
    if (fs->free_space < (size + sizeof(file_info))) {
        printf("Error: Not enough space in file system");
        return;
    }
    for (int i = 0; i < NUM_FILES; i++) {
        if (strcmp(fs->files[i].name, "") == 0) { // Find first available spot in files array
            fs->free_space -= (size + sizeof(file_info));
            strcpy(fs->files[i].name, name);
            fs->files[i].size = size;
            for (int j = 0; j < NUM_BLOCKS; j++) {
                if (strcmp(fs->data[j], "") == 0) { // Find first available block in data array
                    fs->files[i].start_block = j;
                    write_block(fs, j, ""); // Initialize block with empty string
                    break;
                }
            }
            break;
        }
    }
}

// Writes data to the given file in the file system
void write_file(file_system* fs, char* name, char* data) {
    int file_index = -1;
    for (int i = 0; i < NUM_FILES; i++) {
        if (strcmp(fs->files[i].name, name) == 0) { // Find file with given name
            file_index = i;
            break;
        }
    }
    if (file_index == -1) {
        printf("Error: File not found");
        return;
    }
    if (strlen(data) > fs->files[file_index].size) {
        printf("Error: Data too large for file size\n");
        return;
    }
    int num_blocks_needed = (fs->files[file_index].size + BLOCK_SIZE - 1) / BLOCK_SIZE; // Round up to nearest block
    int num_data_blocks = NUM_BLOCKS - NUM_FILES;
    if (num_blocks_needed > num_data_blocks) {
        printf("Error: File too large for file system\n");
        return;
    }
    int blocks_written = 0;
    int block_num = fs->files[file_index].start_block;
    while (blocks_written < num_blocks_needed) { // Write data to required number of blocks
        write_block(fs, block_num, data);
        blocks_written++;
        block_num++;
    }
}

// Reads data from the given file in the file system into the given buffer
void read_file(file_system* fs, char* name, char* buffer) {
    int file_index = -1;
    for (int i = 0; i < NUM_FILES; i++) {
        if (strcmp(fs->files[i].name, name) == 0) { // Find file with given name
            file_index = i;
            break;
        }
    }
    if (file_index == -1) {
        printf("Error: File not found");
        return;
    }
    int num_blocks_needed = (fs->files[file_index].size + BLOCK_SIZE - 1) / BLOCK_SIZE; // Round up to nearest block
    int blocks_read = 0;
    int block_num = fs->files[file_index].start_block;
    while (blocks_read < num_blocks_needed) { // Read data from required number of blocks
        char block_data[BLOCK_SIZE];
        read_block(fs, block_num, block_data);
        strcat(buffer, block_data);
        blocks_read++;
        block_num++;
    }
}

// Prints a list of all files in the file system
void list_files(file_system* fs) {
    printf("Files in file system:\n");
    for (int i = 0; i < NUM_FILES; i++) {
        if (strcmp(fs->files[i].name, "") != 0) {
            printf("%s (%d bytes)\n", fs->files[i].name, fs->files[i].size);
        }
    }
}

int main() {
    file_system fs;
    init_file_system(&fs, NUM_BLOCKS * BLOCK_SIZE, NUM_FILES);
    add_file(&fs, "file1", 100);
    add_file(&fs, "file2", 200);
    list_files(&fs);
    char data[] = "This is some example data";
    write_file(&fs, "file1", data);
    char buffer[MAX_FILE_SIZE] = "";
    read_file(&fs, "file1", buffer);
    printf("File contents: %s\n", buffer);
    return 0;
}