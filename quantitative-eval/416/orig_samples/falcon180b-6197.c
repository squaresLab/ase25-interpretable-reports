//Falcon-180B DATASET v1.0 Category: Phone Book ; Style: lively
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENTRIES 100 // Maximum number of entries in the phone book

struct entry {
    char name[50]; // Name of the person
    char phone[15]; // Phone number of the person
};

void add_entry(struct entry *phonebook, int size) {
    printf("Enter name: ");
    scanf("%s", phonebook[size].name);
    printf("Enter phone number: ");
    scanf("%s", phonebook[size].phone);
}

void search_entry(struct entry *phonebook, int size, char *name) {
    for (int i = 0; i < size; i++) {
        if (strcmp(phonebook[i].name, name) == 0) {
            printf("Name: %s\nPhone number: %s\n", phonebook[i].name, phonebook[i].phone);
            return;
        }
    }
    printf("Entry not found.\n");
}

void delete_entry(struct entry *phonebook, int *size, char *name) {
    for (int i = 0; i < *size; i++) {
        if (strcmp(phonebook[i].name, name) == 0) {
            for (int j = i; j < *size - 1; j++) {
                strcpy(phonebook[j].name, phonebook[j+1].name);
                strcpy(phonebook[j].phone, phonebook[j+1].phone);
            }
            (*size)--;
            return;
        }
    }
    printf("Entry not found.\n");
}

int main() {
    struct entry phonebook[MAX_ENTRIES];
    int size = 0;

    printf("Phone Book\n");
    printf("--------------------\n");

    while (1) {
        printf("1. Add entry\n2. Search entry\n3. Delete entry\n4. Exit\n");
        int choice;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (size >= MAX_ENTRIES) {
                    printf("Phone book is full.\n");
                } else {
                    add_entry(phonebook, size);
                    size++;
                }
                break;
            case 2:
                printf("Enter name to search: ");
                char name[50];
                scanf("%s", name);
                search_entry(phonebook, size, name);
                break;
            case 3:
                printf("Enter name to delete: ");
                scanf("%s", name);
                delete_entry(phonebook, &size, name);
                break;
            case 4:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}