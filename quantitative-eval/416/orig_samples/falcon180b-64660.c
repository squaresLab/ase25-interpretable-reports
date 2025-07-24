//Falcon-180B DATASET v1.0 Category: Fibonacci Sequence Visualizer ; Style: multiplayer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_NUM_THREADS 10
#define MAX_NUM_FIBONACCI 100
#define NUM_THREADS 2
#define NUM_FIBONACCI 50

// Shared data between threads
int fibonacci_numbers[MAX_NUM_FIBONACCI];
int num_fibonacci;

// Semaphore for synchronization
sem_t semaphore;

// Function to generate Fibonacci sequence
void *generate_fibonacci(void *arg) {
    int start_index = *(int *) arg;
    int end_index = start_index + (NUM_FIBONACCI / NUM_THREADS);

    for (int i = start_index; i < end_index; i++) {
        if (fibonacci_numbers[i] == 0) {
            sem_wait(&semaphore);

            int prev_num = (i == 0)? 0 : fibonacci_numbers[i - 1];
            int curr_num = (i == 0)? 1 : fibonacci_numbers[i - 2] + prev_num;
            fibonacci_numbers[i] = curr_num;

            sem_post(&semaphore);
        }
    }

    return NULL;
}

// Main function
int main() {
    int thread_ids[NUM_THREADS];
    pthread_t threads[NUM_THREADS];

    // Initialize semaphore
    sem_init(&semaphore, 0, 1);

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        int start_index = i * (NUM_FIBONACCI / NUM_THREADS);
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, generate_fibonacci, &start_index);
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print Fibonacci sequence
    printf("Fibonacci sequence:\n");
    for (int i = 0; i < num_fibonacci; i++) {
        printf("%d ", fibonacci_numbers[i]);
    }

    return 0;
}