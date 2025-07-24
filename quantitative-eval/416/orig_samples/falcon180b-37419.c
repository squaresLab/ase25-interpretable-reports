//Falcon-180B DATASET v1.0 Category: Threading Library Implementation ; Style: Dennis Ritchie
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Mutex for critical section
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Condition variable for signaling between threads
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Number of threads
#define NUM_THREADS 5

// Function to be executed by each thread
void *thread_function(void *arg) {
    int thread_id = *(int *) arg;

    printf("Thread %d started\n", thread_id);

    // Lock the mutex before entering critical section
    pthread_mutex_lock(&mutex);

    // Signal the main thread that this thread is ready
    pthread_cond_signal(&cond);

    // Unlock the mutex after exiting critical section
    pthread_mutex_unlock(&mutex);

    printf("Thread %d finished\n", thread_id);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS] = {1, 2, 3, 4, 5};

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    // Wait for all threads to signal readiness
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_cond_wait(&cond, &mutex);
    }

    // Print message and exit
    printf("All threads finished\n");

    return 0;
}