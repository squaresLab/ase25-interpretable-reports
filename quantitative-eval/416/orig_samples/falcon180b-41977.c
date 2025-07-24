//Falcon-180B DATASET v1.0 Category: Cafe Billing System ; Style: multi-threaded
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 3
#define MAX_ITEMS 10
#define MAX_PRICE 100

typedef struct {
    char name[20];
    int price;
} MenuItem;

void *customer(void *arg) {
    int id = *(int *) arg;
    printf("Customer %d is ordering...\n", id);
    sleep(1);
    printf("Customer %d ordered %s for $%d\n", id, "Burger", 5);
    return NULL;
}

void *cook(void *arg) {
    int id = *(int *) arg;
    printf("Cook %d is cooking...\n", id);
    sleep(2);
    printf("Cook %d finished cooking %s\n", id, "Burger");
    return NULL;
}

void *waiter(void *arg) {
    int id = *(int *) arg;
    printf("Waiter %d is serving...\n", id);
    sleep(1);
    printf("Waiter %d served %s to Customer %d\n", id, "Burger", id);
    return NULL;
}

void *billing(void *arg) {
    int id = *(int *) arg;
    printf("Billing for Customer %d...\n", id);
    sleep(1);
    printf("Total bill for Customer %d is $%d\n", id, 5);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int i;

    // Initialize menu items
    MenuItem menu[MAX_ITEMS] = {{"Burger", 5}, {"Fries", 3}, {"Drink", 2}};

    // Create threads
    for (i = 0; i < NUM_THREADS; i++) {
        int customer_id = i + 1;
        pthread_create(&threads[i], NULL, customer, &customer_id);
        sleep(1);
        pthread_create(&threads[i], NULL, cook, &customer_id);
        sleep(1);
        pthread_create(&threads[i], NULL, waiter, &customer_id);
        sleep(1);
        pthread_create(&threads[i], NULL, billing, &customer_id);
    }

    // Join threads
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}