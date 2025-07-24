//GEMINI-pro DATASET v1.0 Category: Bitwise operations ; Style: asynchronous
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// Define the number of threads
#define NUM_THREADS 4

// Define the semaphore
sem_t semaphore;

// Define the thread function
void *thread_function(void *arg) {
    // Get the thread number
    int thread_num = *(int *)arg;

    // Acquire the semaphore
    sem_wait(&semaphore);

    // Perform some bitwise operations
    int a = 10;
    int b = 5;
    int c = a & b;
    int d = a | b;
    int e = a ^ b;
    int f = ~a;

    // Print the results
    printf("Thread %d: a & b = %d\n", thread_num, c);
    printf("Thread %d: a | b = %d\n", thread_num, d);
    printf("Thread %d: a ^ b = %d\n", thread_num, e);
    printf("Thread %d: ~a = %d\n", thread_num, f);

    // Release the semaphore
    sem_post(&semaphore);

    // Exit the thread
    pthread_exit(NULL);
}

// Main function
int main() {
    // Initialize the semaphore
    sem_init(&semaphore, 0, 1);

    // Create the threads
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_function, (void *)&i);
    }

    // Join the threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the semaphore
    sem_destroy(&semaphore);

    return 0;
}