//Falcon-180B DATASET v1.0 Category: User Input Sanitizer ; Style: Dennis Ritchie
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 1000

// Function to sanitize user input
void sanitize_input(char *input) {
    int i;
    for (i = 0; i < strlen(input); i++) {
        if (!isalnum(input[i])) {
            input[i] = '\0';
        }
    }
}

// Function to validate user input
int validate_input(char *input) {
    if (strlen(input) == 0) {
        return 0;
    }
    return 1;
}

// Function to get user input
char *get_input(char *prompt) {
    char input[MAX_INPUT_LENGTH];
    printf("%s", prompt);
    fgets(input, MAX_INPUT_LENGTH, stdin);
    input[strcspn(input, "\n")] = '\0';
    sanitize_input(input);
    return input;
}

int main() {
    char name[MAX_INPUT_LENGTH];
    char address[MAX_INPUT_LENGTH];
    char phone_number[MAX_INPUT_LENGTH];

    // Get user input for name
    printf("Enter your name: ");
    strcpy(name, get_input(""));
    printf("Your name is: %s\n", name);

    // Get user input for address
    printf("Enter your address: ");
    strcpy(address, get_input(""));
    printf("Your address is: %s\n", address);

    // Get user input for phone number
    printf("Enter your phone number: ");
    strcpy(phone_number, get_input(""));
    printf("Your phone number is: %s\n", phone_number);

    return 0;
}