//FormAI DATASET v1.0 Category: Database simulation ; Style: active
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_INFO_LENGTH 100

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char info[MAX_INFO_LENGTH];
} Record;

int get_record_size() {
    // Returns the size of the database record.
    return sizeof(int) + MAX_NAME_LENGTH + MAX_INFO_LENGTH;
}

void add_record(char* filename) {
    FILE* file = fopen(filename, "ab");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Record record;
    // Prompt user to enter record information
    printf("Enter the following record information:\n");
    printf("ID: ");
    scanf("%d", &record.id);
    printf("Name: ");
    scanf("%s", record.name);
    printf("Info: ");
    getchar();
    fgets(record.info, MAX_INFO_LENGTH, stdin);

    // Write record to file
    fwrite(&record.id, sizeof(int), 1, file);
    fwrite(record.name, sizeof(char), MAX_NAME_LENGTH, file);
    fwrite(record.info, sizeof(char), MAX_INFO_LENGTH, file);

    fclose(file);
    printf("Record added successfully.\n");
}

void display_record(Record record) {
    printf("ID: %d\n", record.id);
    printf("Name: %s\n", record.name);
    printf("Info: %s", record.info);
}

void display_all_records(char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Record record;
    while (fread(&record.id, sizeof(int), 1, file) != 0) {
        fread(record.name, sizeof(char), MAX_NAME_LENGTH, file);
        fread(record.info, sizeof(char), MAX_INFO_LENGTH, file);
        display_record(record);
        printf("\n");
    }

    fclose(file);
}

void find_record(char* filename, int id) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    Record record;
    while (fread(&record.id, sizeof(int), 1, file) != 0) {
        fread(record.name, sizeof(char), MAX_NAME_LENGTH, file);
        fread(record.info, sizeof(char), MAX_INFO_LENGTH, file);
        if (record.id == id) {
            display_record(record);
            printf("\n");
            fclose(file);
            return;
        }
    }

    fclose(file);
    printf("Record with ID %d not found.\n", id);
}

void delete_record(char* filename, int id) {
    // Create a temporary file to store all the valid records
    char temp_filename[] = "temp.db";
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    FILE* temp_file = fopen(temp_filename, "wb");
    if (temp_file == NULL) {
        printf("Error creating temporary file.\n");
        fclose(file);
        return;
    }

    Record record;
    while (fread(&record.id, sizeof(int), 1, file) != 0) {
        fread(record.name, sizeof(char), MAX_NAME_LENGTH, file);
        fread(record.info, sizeof(char), MAX_INFO_LENGTH, file);
        if (record.id != id) {
            fwrite(&record.id, sizeof(int), 1, temp_file);
            fwrite(record.name, sizeof(char), MAX_NAME_LENGTH, temp_file);
            fwrite(record.info, sizeof(char), MAX_INFO_LENGTH, temp_file);
        }
    }

    fclose(file);
    fclose(temp_file);
    remove(filename);
    rename(temp_filename, filename);
    printf("Record with ID %d deleted successfully.\n", id);
}

int main() {
    char filename[] = "database.db";
    int record_size = get_record_size();

    while (1) {
        // Display options to the user
        printf("\nSelect an option:\n");
        printf("1. Add a record\n");
        printf("2. Display all records\n");
        printf("3. Find a record\n");
        printf("4. Delete a record\n");
        printf("5. Quit\n");
        int option;
        scanf("%d", &option);

        switch (option) {
            case 1:
                add_record(filename);
                break;
            case 2:
                printf("\nAll Records:\n");
                display_all_records(filename);
                break;
            case 3:
                printf("\nEnter the ID of the record to find: ");
                int id;
                scanf("%d", &id);
                find_record(filename, id);
                break;
            case 4:
                printf("\nEnter the ID of the record to delete: ");
                scanf("%d", &id);
                delete_record(filename, id);
                break;
            case 5:
                printf("Exiting program.\n");
                exit(0);
                break;
            default:
                printf("Invalid option.\n");
        }
    }

    return 0;
}