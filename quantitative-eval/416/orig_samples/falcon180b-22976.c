//Falcon-180B DATASET v1.0 Category: User Input Sanitizer ; Style: medieval
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LENGTH 100

// Function to sanitize user input
void sanitize_input(char* input) {
    int i = 0;
    while (i < strlen(input)) {
        if (!isalpha(input[i])) {
            input[i] = '\0';
        }
        i++;
    }
}

// Function to get user input
char* get_input(char* prompt) {
    char input[MAX_INPUT_LENGTH];
    printf("%s: ", prompt);
    fgets(input, MAX_INPUT_LENGTH, stdin);
    input[strcspn(input, "\n")] = '\0';
    return input;
}

// Main function
int main() {
    char name[MAX_INPUT_LENGTH];
    char kingdom[MAX_INPUT_LENGTH];
    char title[MAX_INPUT_LENGTH];

    // Get user input
    printf("Enter thy name, noble knight:\n");
    strcpy(name, get_input("Name: "));
    sanitize_input(name);

    printf("From which kingdom dost thou hail?\n");
    strcpy(kingdom, get_input("Kingdom: "));
    sanitize_input(kingdom);

    printf("What title dost thou bear?\n");
    strcpy(title, get_input("Title: "));
    sanitize_input(title);

    // Print user input
    printf("\nHail, Sir %s of %s!\n", name, kingdom);
    printf("We art pleased to bestow upon thee the title of %s.\n", title);

    return 0;
}