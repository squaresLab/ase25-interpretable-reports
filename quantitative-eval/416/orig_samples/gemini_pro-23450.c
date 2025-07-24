//GEMINI-pro DATASET v1.0 Category: Email Client ; Style: cheerful
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_EMAIL_SIZE 1024
#define MAX_EMAILS 10

typedef struct Email {
    char sender[64];
    char recipient[64];
    char subject[64];
    char body[MAX_EMAIL_SIZE];
} Email;

Email inbox[MAX_EMAILS];
int inbox_count = 0;

void send_email(char *sender, char *recipient, char *subject, char *body) {
    if (inbox_count >= MAX_EMAILS) {
        printf("Sorry, your inbox is full. Please delete some emails to make room for new ones.\n");
        return;
    }

    strcpy(inbox[inbox_count].sender, sender);
    strcpy(inbox[inbox_count].recipient, recipient);
    strcpy(inbox[inbox_count].subject, subject);
    strcpy(inbox[inbox_count].body, body);

    inbox_count++;

    printf("Email sent successfully!\n");
}

void read_email(int index) {
    if (index < 0 || index >= inbox_count) {
        printf("Sorry, that email doesn't exist.\n");
        return;
    }

    Email email = inbox[index];

    printf("From: %s\n", email.sender);
    printf("To: %s\n", email.recipient);
    printf("Subject: %s\n", email.subject);
    printf("Body: %s\n", email.body);
}

void delete_email(int index) {
    if (index < 0 || index >= inbox_count) {
        printf("Sorry, that email doesn't exist.\n");
        return;
    }

    for (int i = index; i < inbox_count - 1; i++) {
        inbox[i] = inbox[i + 1];
    }

    inbox_count--;

    printf("Email deleted successfully!\n");
}

int main() {
    int choice;
    char sender[64];
    char recipient[64];
    char subject[64];
    char body[MAX_EMAIL_SIZE];

    while (true) {
        printf("What would you like to do?\n");
        printf("1. Send an email\n");
        printf("2. Read an email\n");
        printf("3. Delete an email\n");
        printf("4. Quit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the sender's email address: ");
                scanf("%s", sender);
                printf("Enter the recipient's email address: ");
                scanf("%s", recipient);
                printf("Enter the subject of the email: ");
                scanf(" %[^\n]", subject);  // Read the subject line with spaces
                printf("Enter the body of the email: ");
                scanf(" %[^\n]", body);  // Read the body with spaces

                send_email(sender, recipient, subject, body);
                break;
            case 2:
                printf("Enter the index of the email you want to read: ");
                int index;
                scanf("%d", &index);

                read_email(index);
                break;
            case 3:
                printf("Enter the index of the email you want to delete: ");
                scanf("%d", &index);

                delete_email(index);
                break;
            case 4:
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please enter a number from 1 to 4.\n");
        }
    }

    return 0;
}