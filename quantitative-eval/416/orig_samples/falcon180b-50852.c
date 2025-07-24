//Falcon-180B DATASET v1.0 Category: Educational ; Style: cheerful
#include <stdio.h>

// function to print a message
void print_message(char* message) {
    printf("%s\n", message);
}

// function to print a cheerful message
void print_cheerful_message(char* message) {
    printf("Yay! %s\n", message);
}

// function to print a sad message
void print_sad_message(char* message) {
    printf("Oh no! %s\n", message);
}

// function to get user input as a string
char* get_user_input() {
    char input[100];
    printf("Enter something: ");
    scanf("%s", input);
    return input;
}

int main() {
    // print a cheerful welcome message
    print_cheerful_message("Welcome to the program!");

    // get user input
    char* user_input = get_user_input();

    // print a message with the user input
    print_message("You entered: ");
    print_cheerful_message(user_input);

    // check if the user input is a palindrome
    int is_palindrome = 1;
    int length = strlen(user_input);
    for(int i = 0; i < length/2; i++) {
        if(user_input[i]!= user_input[length-i-1]) {
            is_palindrome = 0;
            break;
        }
    }
    if(is_palindrome) {
        print_cheerful_message("The input is a palindrome!");
    } else {
        print_sad_message("The input is not a palindrome :(");
    }

    // print a cheerful goodbye message
    print_cheerful_message("Thanks for using the program!");

    return 0;
}