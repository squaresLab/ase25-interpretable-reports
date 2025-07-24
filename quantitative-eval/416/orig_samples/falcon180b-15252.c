//Falcon-180B DATASET v1.0 Category: Expense Tracker ; Style: synchronous
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CATEGORIES 10
#define MAX_ITEMS 100
#define MAX_NAME_LEN 50
#define MAX_PRICE_LEN 10

typedef struct {
    char name[MAX_NAME_LEN];
    char category[MAX_NAME_LEN];
    char price[MAX_PRICE_LEN];
} item_t;

typedef struct {
    char name[MAX_NAME_LEN];
    int num_items;
    item_t items[MAX_ITEMS];
} category_t;

category_t categories[MAX_CATEGORIES];
int num_categories;

void add_category() {
    printf("Enter category name: ");
    scanf("%s", categories[num_categories].name);
    categories[num_categories].num_items = 0;
    num_categories++;
}

void add_item(int category_idx) {
    printf("Enter item name: ");
    scanf("%s", categories[category_idx].items[categories[category_idx].num_items].name);
    printf("Enter item category: ");
    scanf("%s", categories[category_idx].items[categories[category_idx].num_items].category);
    printf("Enter item price: ");
    scanf("%s", categories[category_idx].items[categories[category_idx].num_items].price);
    categories[category_idx].num_items++;
}

void print_category(int category_idx) {
    printf("Category: %s\n", categories[category_idx].name);
    printf("Number of items: %d\n", categories[category_idx].num_items);
    for (int i = 0; i < categories[category_idx].num_items; i++) {
        printf("Item %d:\n", i+1);
        printf("Name: %s\n", categories[category_idx].items[i].name);
        printf("Category: %s\n", categories[category_idx].items[i].category);
        printf("Price: %s\n", categories[category_idx].items[i].price);
        printf("\n");
    }
}

int main() {
    num_categories = 0;
    printf("Welcome to the Expense Tracker!\n");
    while (1) {
        printf("\n");
        printf("1. Add category\n");
        printf("2. Add item\n");
        printf("3. Print summary\n");
        printf("4. Exit\n");
        printf("\n");
        int choice;
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                add_category();
                break;
            case 2:
                printf("Enter category index: ");
                int category_idx;
                scanf("%d", &category_idx);
                if (category_idx >= 0 && category_idx < num_categories) {
                    add_item(category_idx);
                } else {
                    printf("Invalid category index.\n");
                }
                break;
            case 3:
                printf("Enter category index: ");
                scanf("%d", &category_idx);
                if (category_idx >= 0 && category_idx < num_categories) {
                    print_category(category_idx);
                } else {
                    printf("Invalid category index.\n");
                }
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}