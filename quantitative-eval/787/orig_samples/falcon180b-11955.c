//Falcon-180B DATASET v1.0 Category: Database simulation ; Style: Dennis Ritchie
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 100
#define MAX_COLS 100

typedef struct {
    char *name;
    int age;
    float salary;
} Employee;

typedef struct {
    char *name;
    float balance;
} Account;

typedef struct {
    char *name;
    int num_employees;
    float revenue;
} Company;

typedef struct {
    char *name;
    int num_accounts;
    float total_balance;
} Bank;

void create_table(char *filename, int num_rows, int num_cols) {
    FILE *fp = fopen(filename, "w");
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            fprintf(fp, "%s\t", "");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

void insert_row(FILE *fp, int num_cols, char **values) {
    for (int i = 0; i < num_cols; i++) {
        fprintf(fp, "%s\t", values[i]);
    }
    fprintf(fp, "\n");
}

void print_table(FILE *fp) {
    char line[1024];
    while (fgets(line, sizeof(line), fp)!= NULL) {
        printf("%s", line);
    }
}

int main() {
    Employee employees[] = {{"John Doe", 30, 50000}, {"Jane Smith", 28, 60000}, {"Jim Brown", 35, 70000}};
    Account accounts[] = {{"Checking", 10000}, {"Savings", 20000}};
    Company companies[] = {{"Acme Corp", 50, 1000000}, {"Widget Inc", 75, 2000000}};
    Bank banks[] = {{"Bank of America", 2, 30000}, {"Chase Bank", 3, 50000}};

    create_table("employees.txt", 3, 3);
    insert_row("employees.txt", 3, (char **)employees);

    create_table("accounts.txt", 2, 2);
    insert_row("accounts.txt", 2, (char **)accounts);

    create_table("companies.txt", 2, 3);
    insert_row("companies.txt", 3, (char **)companies);

    create_table("banks.txt", 2, 3);
    insert_row("banks.txt", 3, (char **)banks);

    print_table("employees.txt");
    print_table("accounts.txt");
    print_table("companies.txt");
    print_table("banks.txt");

    return 0;
}