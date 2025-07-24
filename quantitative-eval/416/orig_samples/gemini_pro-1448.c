//GEMINI-pro DATASET v1.0 Category: Memory management ; Style: multi-threaded
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

// Define the number of threads
#define NUM_THREADS 4

// Define the size of the shared memory block
#define SHARED_MEM_SIZE 1024

// Define the mutex
pthread_mutex_t mutex;

// Define the shared memory block
char shared_mem[SHARED_MEM_SIZE];

// Define the thread function
void *thread_function(void *arg) {
    // Get the thread ID
    int thread_id = *((int *) arg);

    // Lock the mutex
    pthread_mutex_lock(&mutex);

    // Write to the shared memory block
    sprintf(shared_mem, "Thread %d: %s", thread_id, "Hello, world!");

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    // Create the threads
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_function, (void *) &i);
    }

    // Join the threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the shared memory block
    printf("%s\n", shared_mem);

    return 0;
}