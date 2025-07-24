//Falcon-180B DATASET v1.0 Category: User Input Sanitizer ; Style: systematic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 1000 // Maximum input length

// Function to sanitize user input
void sanitize_input(char* input) {
    int i = 0;
    while (i < strlen(input)) {
        // Check for invalid characters
        if (input[i] == '\0' || input[i] == '\n' || input[i] == '\r' || input[i] == '\\') {
            input[i] = '\0';
        }
        i++;
    }
}

// Function to check if input is valid
int is_valid_input(char* input) {
    int i = 0;
    while (i < strlen(input)) {
        // Check for invalid characters
        if (input[i] == '\0' || input[i] == '\n' || input[i] == '\r' || input[i] == '\\') {
            return 0;
        }
        i++;
    }
    return 1;
}

// Function to get user input
char* get_input(char* prompt) {
    char input[MAX_INPUT];
    printf("%s", prompt);
    fgets(input, MAX_INPUT, stdin);
    sanitize_input(input);
    return input;
}

int main() {
    char name[MAX_INPUT];
    char address[MAX_INPUT];

    // Get user input
    printf("Enter your name: ");
    strcpy(name, get_input(""));
    printf("Enter your address: ");
    strcpy(address, get_input(""));

    // Check if input is valid
    if (is_valid_input(name) && is_valid_input(address)) {
        printf("Name: %s\nAddress: %s\n", name, address);
    } else {
        printf("Invalid input.\n");
    }

    return 0;
}