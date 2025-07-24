//Falcon-180B DATASET v1.0 Category: Threading Library Implementation ; Style: peaceful
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Define the number of threads to create
#define NUM_THREADS 5

// Define the size of the shared data array
#define ARRAY_SIZE 10

// Define the mutex for synchronizing access to the shared data
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Define the shared data array
int shared_data[ARRAY_SIZE];

// Define the function to be executed by each thread
void *thread_function(void *arg) {
    // Get the thread ID
    int thread_id = *(int *) arg;

    // Print a message indicating the start of the thread
    printf("Thread %d started.\n", thread_id);

    // Loop through the shared data array and add 1 to each element
    for (int i = 0; i < ARRAY_SIZE; i++) {
        pthread_mutex_lock(&mutex);
        shared_data[i] += 1;
        pthread_mutex_unlock(&mutex);
    }

    // Print a message indicating the end of the thread
    printf("Thread %d ended.\n", thread_id);

    // Return NULL to indicate successful completion of the thread
    return NULL;
}

int main() {
    // Create an array to store the thread IDs
    pthread_t threads[NUM_THREADS];

    // Create the threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_function, (void *) &i);
    }

    // Wait for the threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the final value of the shared data array
    printf("Final shared data array:\n");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", shared_data[i]);
    }
    printf("\n");

    return 0;
}