//FormAI DATASET v1.0 Category: System administration ; Style: peaceful
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Print welcome message
    printf("Welcome to the peaceful system administration program.\n\n");
    
    // Prompt user to choose an option
    printf("Please choose an option:\n");
    printf("1. Shutdown system\n");
    printf("2. Reboot system\n");
    printf("3. View system status\n");
    printf("4. Exit program\n\n");
    
    int choice;
    
    // Get user input
    printf("Enter your choice: ");
    scanf("%d", &choice);
    
    // Handle user choice
    switch (choice) {
        case 1:
            // Shutdown system
            printf("Are you sure you want to shutdown the system? (y/n)\n");
            
            char confirm;
            scanf(" %c", &confirm);
            
            if (confirm == 'y' || confirm == 'Y') {
                system("shutdown -h now");
            } else {
                printf("Shutdown cancelled.\n");
            }
            
            break;
            
        case 2:
            // Reboot system
            printf("Are you sure you want to reboot the system? (y/n)\n");
            
            scanf(" %c", &confirm);
            
            if (confirm == 'y' || confirm == 'Y') {
                system("reboot");
            } else {
                printf("Reboot cancelled.\n");
            }
            
            break;
            
        case 3:
            // View system status
            system("top");
            
            break;
            
        case 4:
            // Exit program
            printf("Exiting program...\n");
            exit(0);
            
        default:
            // Invalid choice
            printf("Invalid choice. Please try again.\n");
    }
    
    return 0;
}