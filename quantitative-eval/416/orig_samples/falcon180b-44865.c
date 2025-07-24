//Falcon-180B DATASET v1.0 Category: User Input Sanitizer ; Style: multivariable
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_SIZE 100

// Function to sanitize user input
void sanitize_input(char *input) {
    int i = 0;
    while (i < strlen(input)) {
        if (!isalnum(input[i])) {
            input[i] = '\0';
        }
        i++;
    }
}

// Function to get user input
char *get_input(char *prompt) {
    char input[MAX_INPUT_SIZE];
    printf("%s", prompt);
    fgets(input, MAX_INPUT_SIZE, stdin);
    input[strcspn(input, "\n")] = '\0';
    sanitize_input(input);
    return input;
}

int main() {
    char name[MAX_INPUT_SIZE];
    char address[MAX_INPUT_SIZE];
    char phone_number[MAX_INPUT_SIZE];

    // Get user input for name
    printf("Enter your name: ");
    strcpy(name, get_input(""));

    // Get user input for address
    printf("Enter your address: ");
    strcpy(address, get_input(""));

    // Get user input for phone number
    printf("Enter your phone number: ");
    strcpy(phone_number, get_input(""));

    // Print sanitized user input
    printf("Name: %s\n", name);
    printf("Address: %s\n", address);
    printf("Phone Number: %s\n", phone_number);

    return 0;
}