//GEMINI-pro DATASET v1.0 Category: Threading Library Implementation ; Style: safe
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Define the number of threads to create
#define NUM_THREADS 5

// Define the thread function
void *thread_function(void *arg) {
    // Get the thread ID
    int thread_id = *(int *)arg;

    // Print a message from the thread
    printf("Hello from thread %d!\n", thread_id);

    // Sleep for a random amount of time
    sleep(rand() % 5);

    // Return NULL
    return NULL;
}

int main() {
    // Initialize the random number generator
    srand(time(NULL));

    // Create an array of thread IDs
    pthread_t thread_ids[NUM_THREADS];

    // Create the threads
    for (int i = 0; i < NUM_THREADS; i++) {
        // Create the thread
        if (pthread_create(&thread_ids[i], NULL, thread_function, &i) != 0) {
            perror("pthread_create");
            return EXIT_FAILURE;
        }
    }

    // Join the threads
    for (int i = 0; i < NUM_THREADS; i++) {
        // Join the thread
        if (pthread_join(thread_ids[i], NULL) != 0) {
            perror("pthread_join");
            return EXIT_FAILURE;
        }
    }

    // Print a message from the main thread
    printf("Hello from the main thread!\n");

    // Return success
    return EXIT_SUCCESS;
}