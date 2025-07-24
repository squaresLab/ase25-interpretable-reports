//Gemma-7B DATASET v1.0 Category: Data structures visualization ; Style: decentralized
#include <stdio.h>
#include <stdlib.h>

#define MAX 10

struct Node {
    int data;
    struct Node* next;
};

struct DecentralizedList {
    struct Node** head;
    int size;
};

void insertAtTail(struct DecentralizedList* list, int data)
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;

    if (list->head == NULL)
    {
        list->head = newNode;
    }
    else
    {
        list->head[list->size] = newNode;
        list->size++;
    }
}

void traverse(struct DecentralizedList* list)
{
    struct Node* current = list->head[0];
    while (current)
    {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main()
{
    struct DecentralizedList* list = (struct DecentralizedList*)malloc(sizeof(struct DecentralizedList));
    list->head = NULL;
    list->size = 0;

    insertAtTail(list, 10);
    insertAtTail(list, 20);
    insertAtTail(list, 30);
    insertAtTail(list, 40);

    traverse(list);

    return 0;
}