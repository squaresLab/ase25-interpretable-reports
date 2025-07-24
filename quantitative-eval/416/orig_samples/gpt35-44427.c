//FormAI DATASET v1.0 Category: Banking Record System ; Style: surrealist
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct bank_record {
  int account_number;
  char account_holder_name[50];
  float balance;
  struct bank_record *next;
} BankRecord;

BankRecord *head = NULL;

void insert_record(int account_number, char *account_holder_name, float balance) {
  BankRecord *new_record = malloc(sizeof(BankRecord));
  new_record->account_number = account_number;
  strcpy(new_record->account_holder_name, account_holder_name);
  new_record->balance = balance;
  if (head == NULL) {
    head = new_record;
    new_record->next = NULL;
  } else {
    new_record->next = head;
    head = new_record;
  }
}

void delete_record(int account_number) {
  BankRecord *current_record = head;
  BankRecord *previous_record = NULL;
  while (current_record != NULL) {
    if (current_record->account_number == account_number) {
      if (previous_record == NULL) {
        head = current_record->next;
      } else {
        previous_record->next = current_record->next;
      }
      free(current_record);
      return;
    }
    previous_record = current_record;
    current_record = current_record->next;
  }
}

void print_records() {
  BankRecord *current_record = head;
  printf("Account Number  Account Holder Name      Balance\n");
  printf("------------------------------------------------\n");
  while (current_record != NULL) {
    printf("%-15d%-25s$%.2f\n", current_record->account_number, current_record->account_holder_name, current_record->balance);
    current_record = current_record->next;
  }
}

int main() {
  srand(time(NULL));
  printf("Welcome to the Surreal Bank Record System!\n");
  printf("Where every account is an imaginary friend.\n\n");
  while (1) {
    printf("What would you like to do?\n");
    printf("1. Add a new account\n");
    printf("2. Delete an existing account\n");
    printf("3. View all accounts\n");
    printf("4. QUIT\n");
    int choice;
    scanf("%d", &choice);
    switch (choice) {
      case 1:
        printf("Please enter the account number:\n");
        int account_number;
        scanf("%d", &account_number);
        printf("Please enter the account holder name:\n");
        char account_holder_name[50];
        scanf("%s", account_holder_name);
        printf("Please enter the starting balance:\n");
        float balance;
        scanf("%f", &balance);
        insert_record(account_number, account_holder_name, balance);
        printf("\nNew account added!\n\n");
        break;
      case 2:
        printf("Please enter the account number to delete:\n");
        scanf("%d", &account_number);
        delete_record(account_number);
        printf("\nAccount deleted!\n\n");
        break;
      case 3:
        printf("\n");
        print_records();
        printf("\n");
        break;
      case 4:
        printf("Thank you for using the Surreal Bank Record System!\n");
        return 0;
      default:
        printf("Invalid choice\n");
        break;
    }
  }
}