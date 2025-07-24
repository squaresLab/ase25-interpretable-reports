//FormAI DATASET v1.0 Category: Email Client ; Style: scientific
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SUBJECT_LENGTH 100
#define MAX_BODY_LENGTH 500

typedef struct email {
    char from[50];
    char to[50];
    char subject[MAX_SUBJECT_LENGTH];
    char body[MAX_BODY_LENGTH];
    bool read;
} Email;

typedef struct node {
    Email data;
    struct node *next;
} Node;

Node *head = NULL;

void addEmail(Email e) {
    Node *newNode = (Node*) malloc(sizeof(Node));
    newNode->data = e;
    newNode->next = NULL;
    if(head == NULL) {
        head = newNode;
    } else {
        Node *currentNode = head;
        while(currentNode->next != NULL) {
            currentNode = currentNode->next;
        }
        currentNode->next = newNode;
    }
}

void listEmails() {
    if(head == NULL) {
        printf("You have no emails.\n");
    } else {
        Node *currentNode = head;
        int index = 1;
        while(currentNode != NULL) {
            printf("%d. %s\n", index, currentNode->data.subject);
            currentNode = currentNode->next;
            index++;
        }
    }
}

void readEmail(int index) {
    if(head == NULL) {
        printf("You have no emails.\n");
    } else {
        int current = 1;
        Node *currentNode = head;
        while(currentNode != NULL && current != index) {
            currentNode = currentNode->next;
            current++;
        }
        if(currentNode == NULL) {
            printf("Invalid email index.\n");
        } else {
            printf("From: %s\n", currentNode->data.from);
            printf("To: %s\n", currentNode->data.to);
            printf("Subject: %s\n", currentNode->data.subject);
            printf("Body: %s\n", currentNode->data.body);
            currentNode->data.read = true;
        }
    }
}

void deleteEmail(int index) {
    if(head == NULL) {
        printf("You have no emails.\n");
    } else {
        int current = 1;
        Node *currentNode = head;
        Node *previousNode = NULL;
        while(currentNode != NULL && current != index) {
            previousNode = currentNode;
            currentNode = currentNode->next;
            current++;
        }
        if(currentNode == NULL) {
            printf("Invalid email index.\n");
        } else {
            if(previousNode == NULL) {
                head = currentNode->next;
            } else {
                previousNode->next = currentNode->next;
            }
            free(currentNode);
        }
    }
}

int main() {
    Email e1 = {"john@example.com", "jane@example.com", "Hello World", "This is a test email.", false};
    Email e2 = {"jane@example.com", "john@example.com", "RE: Hello World", "Thanks for the email.", false};
    Email e3 = {"alice@example.com", "john@example.com", "Meeting Reminder", "Don't forget the meeting tomorrow.", false};
    addEmail(e1);
    addEmail(e2);
    addEmail(e3);
    while(true) {
        printf("\n");
        printf("1. List Emails\n");
        printf("2. Read Email\n");
        printf("3. Delete Email\n");
        printf("4. Quit\n");
        printf("\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);
        printf("\n");
        switch(choice) {
            case 1:
                listEmails();
                break;
            case 2:
                printf("Enter email index: ");
                int index;
                scanf("%d", &index);
                readEmail(index);
                break;
            case 3:
                printf("Enter email index: ");
                scanf("%d", &index);
                deleteEmail(index);
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}