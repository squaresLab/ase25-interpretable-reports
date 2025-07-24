//Code Llama-13B DATASET v1.0 Category: Banking Record System ; Style: funny
// A unique C Banking Record System example program in a funny style

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct account {
  char name[20];
  float balance;
};

int main() {
  struct account accounts[5] = {
    {"John", 1000},
    {"Mary", 2000},
    {"Bob", 3000},
    {"Alice", 4000},
    {"Eve", 5000}
  };

  printf("Welcome to our funny banking system! 😄\n");

  int option;
  do {
    printf("1. Check balance\n");
    printf("2. Deposit money\n");
    printf("3. Withdraw money\n");
    printf("4. Exit\n");

    printf("Enter your choice: ");
    scanf("%d", &option);

    switch (option) {
      case 1:
        printf("Your current balance is: $%.2f\n", accounts[0].balance);
        break;
      case 2:
        printf("Enter the amount to deposit: ");
        float amount;
        scanf("%f", &amount);
        accounts[0].balance += amount;
        printf("Your new balance is: $%.2f\n", accounts[0].balance);
        break;
      case 3:
        printf("Enter the amount to withdraw: ");
        scanf("%f", &amount);
        accounts[0].balance -= amount;
        printf("Your new balance is: $%.2f\n", accounts[0].balance);
        break;
      case 4:
        printf("Thank you for using our funny banking system! 😄\n");
        break;
      default:
        printf("Invalid option. Try again!\n");
    }
  } while (option != 4);

  return 0;
}