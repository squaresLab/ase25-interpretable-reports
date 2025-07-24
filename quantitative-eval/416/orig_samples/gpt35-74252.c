//FormAI DATASET v1.0 Category: Banking Record System ; Style: genious
#include <stdio.h>
#include <stdlib.h>

struct BankRecord {
    int accountNumber;
    char name[30];
    float balance;
};

void main() {
    int choice, i, accountNumber, flag = 0;
    char name[30];
    float balance;

    printf("Welcome to the Genius Banking Record System\n");
    printf("---------------------------------------------\n");

    while (1) {
        printf("\n1. Add a new record");
        printf("\n2. Display all records");
        printf("\n3. Search for a record");
        printf("\n4. Modify a record");
        printf("\n5. Delete a record");
        printf("\n6. Exit");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nEnter the account number: ");
                scanf("%d", &accountNumber);

                printf("Enter the account holder's name: ");
                scanf("%s", name);

                printf("Enter the balance: ");
                scanf("%f", &balance);

                FILE *fp;
                fp = fopen("bank_records.dat", "ab"); // Opening the file in append mode

                struct BankRecord record = {accountNumber, name, balance};
                fwrite(&record, sizeof(record), 1, fp); // Writing the record to the file

                fclose(fp);
                printf("\nRecord added successfully!");
                break;

            case 2:
                fp = fopen("bank_records.dat", "rb"); // Opening the file in read mode

                if (fp == NULL) {
                    printf("\nThe record list is empty.");
                    break;
                }

                printf("\nAccount number\tAccount holder's name\tBalance\n");
                printf("---------------------------------------------------\n");

                while (fread(&record, sizeof(record), 1, fp)) { // Reading records from the file
                    printf("%d\t\t%s\t\t%.2f\n", record.accountNumber, record.name, record.balance);
                }

                fclose(fp);
                break;

            case 3:
                printf("\nEnter the account number to search for: ");
                scanf("%d", &accountNumber);

                fp = fopen("bank_records.dat", "rb");

                while (fread(&record, sizeof(record), 1, fp)) {
                    if (record.accountNumber == accountNumber) {
                        printf("\nAccount number\tAccount holder's name\tBalance\n");
                        printf("---------------------------------------------------\n");
                        printf("%d\t\t%s\t\t%.2f\n", record.accountNumber, record.name, record.balance);
                        flag = 1;
                        break;
                    }
                }

                if (flag == 0) {
                    printf("\nRecord not found.");
                }

                fclose(fp);
                break;

            case 4:
                printf("\nEnter the account number to modify: ");
                scanf("%d", &accountNumber);

                fp = fopen("bank_records.dat", "rb+");
                flag = 0;

                while (fread(&record, sizeof(record), 1, fp)) {
                    if (record.accountNumber == accountNumber) {
                        printf("\nEnter the new account holder's name: ");
                        scanf("%s", record.name);

                        printf("Enter the new balance: ");
                        scanf("%f", &record.balance);

                        fseek(fp, -sizeof(record), SEEK_CUR); // Moving the file pointer back by the size of a record
                        fwrite(&record, sizeof(record), 1, fp); // Updating the record in the file

                        flag = 1;
                        break;
                    }
                }

                if (flag == 0) {
                    printf("\nRecord not found.");
                } else {
                    printf("\nRecord modified successfully!");
                }

                fclose(fp);
                break;

            case 5:
                printf("\nEnter the account number to delete: ");
                scanf("%d", &accountNumber);

                fp = fopen("bank_records.dat", "rb");
                FILE *temp;
                temp = fopen("temp.dat", "wb");

                flag = 0;

                while (fread(&record, sizeof(record), 1, fp)) {
                    if (record.accountNumber != accountNumber) {
                        fwrite(&record, sizeof(record), 1, temp);
                    } else {
                        flag = 1;
                    }
                }

                fclose(fp);
                fclose(temp);

                remove("bank_records.dat"); // Deleting the old file
                rename("temp.dat", "bank_records.dat"); // Renaming the temporary file

                if (flag == 0) {
                    printf("\nRecord not found.");
                } else {
                    printf("\nRecord deleted successfully!");
                }

                break;

            case 6:
                exit(0);

            default:
                printf("\nInvalid choice. Please choose from the menu.");
                break;
        }
    }
}