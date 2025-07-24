//GEMINI-pro DATASET v1.0 Category: Digital Diary ; Style: grateful
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

// Define the maximum length of a diary entry
#define MAX_ENTRY_LENGTH 1024

// Define the maximum number of diary entries
#define MAX_ENTRIES 100

// Define the name of the file where the diary entries will be stored
#define DIARY_FILE "diary.txt"

// Create a struct to store a diary entry
typedef struct diary_entry {
    char date[11];
    char time[9];
    char entry[MAX_ENTRY_LENGTH];
} diary_entry;

// Create an array to store the diary entries
diary_entry entries[MAX_ENTRIES];

// Keep track of the number of diary entries
int num_entries = 0;

// Function to add a new diary entry
void add_entry() {
    // Get the current date and time
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(entries[num_entries].date, sizeof(entries[num_entries].date), "%Y-%m-%d", tm);
    strftime(entries[num_entries].time, sizeof(entries[num_entries].time), "%H:%M:%S", tm);

    // Get the diary entry from the user
    printf("Enter your diary entry: ");
    fgets(entries[num_entries].entry, sizeof(entries[num_entries].entry), stdin);

    // Increment the number of diary entries
    num_entries++;
}

// Function to edit a diary entry
void edit_entry(int index) {
    // Get the diary entry from the user
    printf("Enter your diary entry: ");
    fgets(entries[index].entry, sizeof(entries[index].entry), stdin);
}

// Function to delete a diary entry
void delete_entry(int index) {
    // Shift the diary entries down to fill the gap
    for (int i = index + 1; i < num_entries; i++) {
        entries[i - 1] = entries[i];
    }

    // Decrement the number of diary entries
    num_entries--;
}

// Function to view all diary entries
void view_entries() {
    // Print out the diary entries
    for (int i = 0; i < num_entries; i++) {
        printf("%s %s\n", entries[i].date, entries[i].time);
        printf("%s\n", entries[i].entry);
        printf("\n");
    }
}

// Function to save the diary entries to a file
void save_entries() {
    // Open the diary file for writing
    FILE *fp = fopen(DIARY_FILE, "w");
    if (fp == NULL) {
        printf("Error opening diary file for writing.\n");
        return;
    }

    // Write the diary entries to the file
    for (int i = 0; i < num_entries; i++) {
        fprintf(fp, "%s %s\n", entries[i].date, entries[i].time);
        fprintf(fp, "%s\n", entries[i].entry);
        fprintf(fp, "\n");
    }

    // Close the diary file
    fclose(fp);
}

// Function to load the diary entries from a file
void load_entries() {
    // Open the diary file for reading
    FILE *fp = fopen(DIARY_FILE, "r");
    if (fp == NULL) {
        printf("Error opening diary file for reading.\n");
        return;
    }

    // Read the diary entries from the file
    while (fgets(entries[num_entries].date, sizeof(entries[num_entries].date), fp) != NULL) {
        fgets(entries[num_entries].time, sizeof(entries[num_entries].time), fp);
        fgets(entries[num_entries].entry, sizeof(entries[num_entries].entry), fp);
        num_entries++;
    }

    // Close the diary file
    fclose(fp);
}

// Main function
int main() {
    // Load the diary entries from the file
    load_entries();

    // Get the user's input
    int choice;
    do {
        printf("1. Add a new diary entry\n");
        printf("2. Edit a diary entry\n");
        printf("3. Delete a diary entry\n");
        printf("4. View all diary entries\n");
        printf("5. Save the diary entries to a file\n");
        printf("6. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Call the appropriate function based on the user's choice
        switch (choice) {
            case 1:
                add_entry();
                break;
            case 2:
                printf("Enter the index of the diary entry you want to edit: ");
                int index;
                scanf("%d", &index);
                edit_entry(index);
                break;
            case 3:
                printf("Enter the index of the diary entry you want to delete: ");
                scanf("%d", &index);
                delete_entry(index);
                break;
            case 4:
                view_entries();
                break;
            case 5:
                save_entries();
                break;
        }
    } while (choice != 6);

    return 0;
}