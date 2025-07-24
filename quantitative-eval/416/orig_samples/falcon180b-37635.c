//Falcon-180B DATASET v1.0 Category: Threading Library Implementation ; Style: protected
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Mutex for protecting shared data
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Condition variable for signaling threads
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Flag for indicating whether all threads have finished
int finished = 0;

// Function called by each thread
void *thread_func(void *arg) {
    // Lock the mutex to protect shared data
    pthread_mutex_lock(&mutex);

    // Signal the main thread that a new thread has started
    pthread_cond_signal(&cond);

    // Do some work...
    int id = *((int *) arg);
    printf("Thread %d started.\n", id);
    sleep(1);
    printf("Thread %d finished.\n", id);

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);

    // Signal the main thread that a thread has finished
    pthread_cond_signal(&cond);

    return NULL;
}

int main() {
    // Create an array of thread IDs
    pthread_t threads[5];

    // Create 5 threads
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void *) &i);
    }

    // Lock the mutex to protect shared data
    pthread_mutex_lock(&mutex);

    // Wait for all threads to finish
    while (finished == 0) {
        // Signal the main thread when a thread has started
        pthread_cond_wait(&cond, &mutex);
    }

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);

    return 0;
}