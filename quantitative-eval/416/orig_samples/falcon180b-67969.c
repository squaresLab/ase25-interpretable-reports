//Falcon-180B DATASET v1.0 Category: Threading Library Implementation ; Style: satisfied
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Mutex for thread synchronization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Condition variable for thread synchronization
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

// Number of threads
#define NUM_THREADS 5

// Function to be executed by each thread
void* thread_func(void* arg) {
    int id = *(int*) arg;
    printf("Thread %d started\n", id);

    // Lock the mutex
    pthread_mutex_lock(&mutex);

    // Signal the main thread that a new thread has started
    pthread_cond_signal(&condition);

    // Wait for the main thread to signal that it's time to exit
    pthread_cond_wait(&condition, &mutex);

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);

    printf("Thread %d exited\n", id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void*) &i);
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Main thread exited\n");
    return 0;
}