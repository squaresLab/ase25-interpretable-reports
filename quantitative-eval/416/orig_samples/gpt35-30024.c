//FormAI DATASET v1.0 Category: Mailing list manager ; Style: peaceful
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to store email addresses
typedef struct email {
    char name[50];
    char email_address[100];
    struct email *next;
} email_t;

// Function to add a new email to the mailing list
void add_email(email_t **mailing_list, char *name, char *email_address) {
    // Allocate memory for new email
    email_t *new_email = (email_t *)malloc(sizeof(email_t));
    if (new_email == NULL) {
        printf("Error: Could not allocate memory for new email.\n");
        return;
    }
    // Copy data into new email
    strcpy(new_email->name, name);
    strcpy(new_email->email_address, email_address);
    // Set next as NULL for now
    new_email->next = NULL;
    // If mailing list is empty, set new email as first element
    if (*mailing_list == NULL) {
        *mailing_list = new_email;
    }
    // Otherwise, find last element and set its next as new email
    else {
        email_t *current = *mailing_list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_email;
    }
    printf("Email address added to mailing list.\n");
}

// Function to delete an email from the mailing list
void delete_email(email_t **mailing_list, char *email_address) {
    // If mailing list is empty, do nothing
    if (*mailing_list == NULL) {
        printf("Mailing list is empty.\n");
        return;
    }
    // If first email matches, delete it
    if (strcmp((*mailing_list)->email_address, email_address) == 0) {
        email_t *temp = *mailing_list;
        *mailing_list = (*mailing_list)->next;
        free(temp);
        printf("%s has been removed from the mailing list.\n", email_address);
        return;
    }
    // Otherwise, find the email and delete it
    email_t *current = *mailing_list;
    email_t *previous = NULL;
    while (current != NULL && strcmp(current->email_address, email_address) != 0) {
        previous = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("%s was not found in the mailing list.\n", email_address);
        return;
    }
    previous->next = current->next;
    free(current);
    printf("%s has been removed from the mailing list.\n", email_address);
}

// Function to display all emails in the mailing list
void display_emails(email_t *mailing_list) {
    // If mailing list is empty, do nothing
    if (mailing_list == NULL) {
        printf("Mailing list is empty.\n");
        return;
    }
    printf("Mailing list:\n");
    while (mailing_list != NULL) {
        printf("%s <%s>\n", mailing_list->name, mailing_list->email_address);
        mailing_list = mailing_list->next;
    }
}

int main() {
    email_t *mailing_list = NULL;
    int choice;
    while (1) {
        printf("\nMenu:\n1. Add email address\n2. Delete email address\n3. Display all emails\n4. Exit\nEnter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            printf("Enter name: ");
            char name[50], email_address[100];
            scanf("%s", name);
            printf("Enter email address: ");
            scanf("%s", email_address);
            add_email(&mailing_list, name, email_address);
            break;
        case 2:
            printf("Enter email address to delete: ");
            scanf("%s", email_address);
            delete_email(&mailing_list, email_address);
            break;
        case 3:
            display_emails(mailing_list);
            break;
        case 4:
            printf("Exiting.\n");
            exit(0);
        default:
            printf("Invalid choice.\n");
        }
    }
    return 0;
}