//Falcon-180B DATASET v1.0 Category: Threading Library Implementation ; Style: beginner-friendly
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Mutex for synchronization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Condition variable for signaling
pthread_cond_t condition = PTHREAD_COND_INITIALIZER;

// Number of threads
#define NUM_THREADS 5

// Thread function
void *thread_function(void *arg) {
    int id = *(int*) arg;
    printf("Thread %d started\n", id);

    // Lock the mutex
    pthread_mutex_lock(&mutex);

    // Signal the main thread that a thread has started
    pthread_cond_signal(&condition);

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);

    // Do some work
    for (int i = 0; i < 1000000; i++) {
        printf("Thread %d is working...\n", id);
    }

    return NULL;
}

// Main function
int main() {
    pthread_t threads[NUM_THREADS];

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_function, (void*) &i);
    }

    // Wait for threads to start
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_cond_wait(&condition, &mutex);
    }

    // Unlock the mutex and exit
    pthread_mutex_unlock(&mutex);
    printf("All threads have started\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}