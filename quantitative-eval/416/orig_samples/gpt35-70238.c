//FormAI DATASET v1.0 Category: Password management ; Style: ephemeral
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PASSWORDS 100
#define MAX_LENGTH 50

// Struct for storing username and password
typedef struct {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
} Account;

// Function to generate a random password
char* generatePassword(int length) {
    char* password = malloc(length+1);
    char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    int numChars = strlen(characters);
    srand(time(NULL)); // Seed random number generator with current time

    for (int i = 0; i < length; i++) {
        int randomIndex = rand() % numChars;
        password[i] = characters[randomIndex];
    }

    password[length] = '\0';
    return password;
}

// Function to add a new account to the password manager
void addAccount(Account* passwords, int numPasswords) {
    if (numPasswords >= MAX_PASSWORDS) {
        printf("Cannot add more passwords. Maximum number of passwords reached.\n");
        return;
    }

    printf("Enter username: ");
    scanf("%s", passwords[numPasswords].username);

    printf("Do you want to generate a random password? (y/n) ");
    char choice;
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        char* password = generatePassword(10); // Generate a random 10 character password
        strcpy(passwords[numPasswords].password, password);
        printf("Generated password: %s\n", password);
        free(password);
    } else {
        // Otherwise, ask the user to enter a password
        printf("Enter password: ");
        scanf("%s", passwords[numPasswords].password);
    }

    printf("Account added successfully.\n");
}

// Function to find an account by its username
int findAccount(Account* passwords, int numPasswords, char* username) {
    for (int i = 0; i < numPasswords; i++) {
        if (strcmp(passwords[i].username, username) == 0) {
            return i;
        }
    }

    return -1; // Account not found
}

// Function to delete an account
void deleteAccount(Account* passwords, int numPasswords, char* username) {
    int index = findAccount(passwords, numPasswords, username);

    if (index == -1) {
        printf("Account not found.\n");
        return;
    }

    // Shift all accounts after the deleted account back by one index
    for (int i = index; i < numPasswords-1; i++) {
        passwords[i] = passwords[i+1];
    }

    printf("Account deleted successfully.\n");
}

// Function to change a password
void changePassword(Account* passwords, int numPasswords, char* username) {
    int index = findAccount(passwords, numPasswords, username);

    if (index == -1) {
        printf("Account not found.\n");
        return;
    }

    // Generate a new random password and set it for the account
    char* newPassword = generatePassword(10);
    strcpy(passwords[index].password, newPassword);
    printf("New password: %s\n", newPassword);
    free(newPassword);
}

// Function to print all accounts
void printAccounts(Account* passwords, int numPasswords) {
    if (numPasswords == 0) {
        printf("No passwords stored.\n");
        return;
    }

    printf("Accounts:\n");

    for (int i = 0; i < numPasswords; i++) {
        printf("%s: %s\n", passwords[i].username, passwords[i].password);
    }
}

int main() {
    Account passwords[MAX_PASSWORDS]; // Array to store accounts
    int numPasswords = 0; // Keep track of number of accounts stored

    // Loop infinitely until the user chooses to exit
    while (true) {
        printf("\n*** PASSWORD MANAGER ***\n");
        printf("1. Add account\n");
        printf("2. Delete account\n");
        printf("3. Change password\n");
        printf("4. Show accounts\n");
        printf("5. Exit\n");
        printf("Enter choice (1-5): ");

        int choice;
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                addAccount(passwords, numPasswords);
                numPasswords++;
                break;
            case 2:
                printf("Enter username of account to delete: ");
                char username[MAX_LENGTH];
                scanf("%s", username);
                deleteAccount(passwords, numPasswords, username);
                numPasswords--;
                break;
            case 3:
                printf("Enter username of account to change password: ");
                scanf("%s", username);
                changePassword(passwords, numPasswords, username);
                break;
            case 4:
                printAccounts(passwords, numPasswords);
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice. Please enter a number between 1 and 5.\n");
        }
    }

    return 0;
}