//FormAI DATASET v1.0 Category: Expense Tracker ; Style: mathematical
#include <stdio.h>
#include <stdlib.h>

// Define maximum number of expenses and categories
#define MAX_EXPENSES 100
#define MAX_CATEGORIES 10

// Define struct for expense
struct Expense {
    int category;
    double amount;
    char description[50];
};

// Function to display menu 
void displayMenu() {
    printf("\n======= EXPENSE TRACKER MENU =======");
    printf("\n1. Add Expense");
    printf("\n2. View Expenses By Category");
    printf("\n3. View Total Expenses By Category");
    printf("\n4. View Total Expenses");
    printf("\n5. Exit");
}

int main() {
    // Initialize array of expenses
    struct Expense expenses[MAX_EXPENSES];
    // Initialize array of category totals
    double categoryTotals[MAX_CATEGORIES] = {0};
    // Initialize total expense variable
    double totalExpense = 0;
    // Initialize number of expenses entered counter
    int numExpenses = 0;
    // Initialize main loop variable
    int choice = 0;
    
    // Main loop
    do {
        // Display menu
        displayMenu();
        // Get user's choice
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: // Add Expense
                // Check if maximum number of expenses have been entered
                if(numExpenses == MAX_EXPENSES) {
                    printf("\nMaximum number of expenses reached.");
                    break;
                }
                // Get expense category
                int category;
                printf("\nEnter expense category (1-%d): ", MAX_CATEGORIES);
                scanf("%d", &category);
                // Check if category is within range
                if(category < 1 || category > MAX_CATEGORIES) {
                    printf("\nInvalid category.");
                    break;
                }
                // Get expense amount
                double amount;
                printf("Enter expense amount: $");
                scanf("%lf", &amount);
                // Get expense description
                char description[50];
                printf("Enter expense description: ");
                scanf("%s", description);
                // Update total expense variable and category total
                totalExpense += amount;
                categoryTotals[category-1] += amount;
                // Create expense struct and add to array
                struct Expense expense = {category, amount, description};
                expenses[numExpenses++] = expense;
                printf("\nExpense added successfully.");
                break;
            case 2: // View Expenses By Category
                // Get category to view
                printf("\nEnter category to view (1-%d): ", MAX_CATEGORIES);
                scanf("%d", &category);
                // Print header
                printf("\nExpenses for Category %d:", category);
                printf("\n==========================");
                // Loop through array of expenses and print those with selected category
                for(int i = 0; i < numExpenses; i++) {
                    if(expenses[i].category == category) {
                        printf("\nAmount: $%.2lf\nDescription: %s", expenses[i].amount, expenses[i].description);
                    }
                }
                break;
            case 3: // View Total Expenses By Category
                // Print header
                printf("\nTotal Expenses By Category:");
                printf("\n===========================");
                // Loop through category totals array and print each category's total
                for(int i = 0; i < MAX_CATEGORIES; i++) {
                    printf("\nCategory %d Total: $%.2lf", i+1, categoryTotals[i]);
                }
                break;
            case 4: // View Total Expenses
                printf("\nTotal Expenses: $%.2lf", totalExpense);
                break;
            case 5: // Exit
                printf("\nExiting program...");
                break;
            default:
                printf("\nInvalid choice.");
        }
    } while(choice != 5);
    
    return 0;
}