//FormAI DATASET v1.0 Category: Personal Finance Planner ; Style: Alan Touring
#include <stdio.h>

int main() {
   float income, expenses, savings;
   
   printf("Enter your monthly income: ");
   scanf("%f", &income);
   
   printf("Enter your monthly expenses: ");
   scanf("%f", &expenses);
   
   savings = income - expenses;
   
   printf("Your monthly savings: %.2f\n", savings);
   
   printf("\n*** Personal Finance Planner ***\n");
   
   printf("1. Calculate total income\n");
   printf("2. Calculate total expenses\n");
   printf("3. Calculate savings rate\n");
   printf("4. Exit\n");
   
   int choice;
   
   do {
      printf("\nEnter your choice: ");
      scanf("%d", &choice);
      
      switch(choice) {
         case 1:
            printf("Enter number of months: ");
            int months;
            scanf("%d", &months);
            printf("Total income for %d months: %.2f\n", months, income * months);
            break;
            
         case 2:
            printf("Enter number of months: ");
            scanf("%d", &months);
            printf("Total expenses for %d months: %.2f\n", months, expenses * months);
            break;
            
         case 3:
            printf("Savings rate: %.2f%%\n", (savings / income) * 100);
            break;
            
         case 4:
            printf("Goodbye.\n");
            break;
            
         default:
            printf("Invalid choice. Please try again.\n");
      }
   } while(choice != 4);
   
   return 0;
}