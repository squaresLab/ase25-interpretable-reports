//GEMINI-pro DATASET v1.0 Category: Email Client ; Style: peaceful
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Structure to represent an email
typedef struct email {
    char *sender;
    char *recipient;
    char *subject;
    char *body;
} Email;

// Function to create a new email
Email *create_email(char *sender, char *recipient, char *subject, char *body) {
    Email *email = (Email *)malloc(sizeof(Email));
    email->sender = sender;
    email->recipient = recipient;
    email->subject = subject;
    email->body = body;
    return email;
}

// Function to send an email
void send_email(Email *email) {
    printf("From: %s\n", email->sender);
    printf("To: %s\n", email->recipient);
    printf("Subject: %s\n", email->subject);
    printf("Body: %s\n", email->body);
}

// Function to read an email from a file
Email *read_email_from_file(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        return NULL;
    }

    // Read the sender
    char sender[100];
    fgets(sender, 100, fp);

    // Read the recipient
    char recipient[100];
    fgets(recipient, 100, fp);

    // Read the subject
    char subject[100];
    fgets(subject, 100, fp);

    // Read the body
    char body[1000];
    fgets(body, 1000, fp);

    // Create the email
    Email *email = create_email(sender, recipient, subject, body);

    // Close the file
    fclose(fp);

    return email;
}

// Function to save an email to a file
void save_email_to_file(Email *email, char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return;
    }

    // Write the sender
    fprintf(fp, "%s\n", email->sender);

    // Write the recipient
    fprintf(fp, "%s\n", email->recipient);

    // Write the subject
    fprintf(fp, "%s\n", email->subject);

    // Write the body
    fprintf(fp, "%s\n", email->body);

    // Close the file
    fclose(fp);
}

// Function to print the menu
void print_menu() {
    printf("1. Create a new email\n");
    printf("2. Send an email\n");
    printf("3. Read an email from a file\n");
    printf("4. Save an email to a file\n");
    printf("5. Quit\n");
}

// Function to get the user's choice
int get_user_choice() {
    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);
    return choice;
}

// Main function
int main() {
    // Create an email client
    Email *email = NULL;

    // Loop until the user quits
    while (true) {
        // Print the menu
        print_menu();

        // Get the user's choice
        int choice = get_user_choice();

        // Handle the user's choice
        switch (choice) {
            case 1:
                // Create a new email
                email = create_email(NULL, NULL, NULL, NULL);
                printf("Enter the sender: ");
                scanf("%s", email->sender);
                printf("Enter the recipient: ");
                scanf("%s", email->recipient);
                printf("Enter the subject: ");
                scanf("%s", email->subject);
                printf("Enter the body: ");
                scanf("%s", email->body);
                break;
            case 2:
                // Send an email
                send_email(email);
                break;
            case 3:
                // Read an email from a file
                printf("Enter the filename: ");
                char filename[100];
                scanf("%s", filename);
                email = read_email_from_file(filename);
                if (email == NULL) {
                    printf("Error: could not read email from file\n");
                } else {
                    printf("Email read from file:\n");
                    send_email(email);
                }
                break;
            case 4:
                // Save an email to a file
                printf("Enter the filename: ");
                scanf("%s", filename);
                save_email_to_file(email, filename);
                printf("Email saved to file\n");
                break;
            case 5:
                // Quit
                return 0;
            default:
                // Invalid choice
                printf("Invalid choice\n");
        }
    }

    return 0;
}