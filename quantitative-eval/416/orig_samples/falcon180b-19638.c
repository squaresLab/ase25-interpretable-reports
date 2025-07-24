//Falcon-180B DATASET v1.0 Category: Banking Record System ; Style: multi-threaded
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_ACCOUNTS 5
#define NUM_THREADS 10

typedef struct {
    int id;
    char name[50];
    float balance;
} Account;

Account accounts[NUM_ACCOUNTS] = {
    {1, "John Doe", 1000.00},
    {2, "Jane Doe", 2000.00},
    {3, "Bob Smith", 3000.00},
    {4, "Alice Johnson", 4000.00},
    {5, "Charlie Brown", 5000.00}
};

void *deposit(void *arg) {
    int account_id = *(int *)arg;
    Account *account = &accounts[account_id - 1];

    printf("Depositing $100 into account %d (%s)\n", account_id, account->name);
    account->balance += 100.00;
}

void *withdraw(void *arg) {
    int account_id = *(int *)arg;
    Account *account = &accounts[account_id - 1];

    printf("Withdrawing $100 from account %d (%s)\n", account_id, account->name);
    account->balance -= 100.00;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int i;

    for (i = 0; i < NUM_THREADS; i++) {
        int account_id = rand() % NUM_ACCOUNTS + 1;
        pthread_create(&threads[i], NULL, i % 2 == 0? deposit : withdraw, (void *)&account_id);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}