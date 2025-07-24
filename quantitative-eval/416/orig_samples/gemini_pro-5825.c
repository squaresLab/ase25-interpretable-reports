//GEMINI-pro DATASET v1.0 Category: Password management ; Style: rigorous
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PASSWORDS 100
#define MAX_PASSWORD_LENGTH 20

typedef struct {
    char password[MAX_PASSWORD_LENGTH];
    int length;
} Password;

char **passwords;
int num_passwords = 0;

void add_password(char *password) {
    if (num_passwords >= MAX_PASSWORDS) {
        printf("Error: Maximum number of passwords reached.\n");
        return;
    }

    passwords[num_passwords] = malloc(strlen(password) + 1);
    strcpy(passwords[num_passwords], password);
    passwords[num_passwords][strlen(password)] = '\0';
    num_passwords++;
}

int find_password(char *password) {
    for (int i = 0; i < num_passwords; i++) {
        if (strcmp(passwords[i], password) == 0) {
            return i;
        }
    }

    return -1;
}

void delete_password(int index) {
    if (index < 0 || index >= num_passwords) {
        printf("Error: Invalid password index.\n");
        return;
    }

    free(passwords[index]);
    for (int i = index; i < num_passwords - 1; i++) {
        passwords[i] = passwords[i + 1];
    }
    num_passwords--;
}

void print_passwords() {
    printf("Passwords:\n");
    for (int i = 0; i < num_passwords; i++) {
        printf("%d: %s\n", i, passwords[i]);
    }
}

int main() {
    passwords = malloc(sizeof(char *) * MAX_PASSWORDS);
    int choice;
    char password[MAX_PASSWORD_LENGTH];

    while (1) {
        printf("1. Add password\n");
        printf("2. Find password\n");
        printf("3. Delete password\n");
        printf("4. Print passwords\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter password: ");
                scanf("%s", password);
                add_password(password);
                break;
            case 2:
                printf("Enter password: ");
                scanf("%s", password);
                int index = find_password(password);
                if (index == -1) {
                    printf("Password not found.\n");
                } else {
                    printf("Password found at index %d.\n", index);
                }
                break;
            case 3:
                printf("Enter password index: ");
                scanf("%d", &index);
                delete_password(index);
                break;
            case 4:
                print_passwords();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}