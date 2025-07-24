//Falcon-180B DATASET v1.0 Category: Threading Library Implementation ; Style: post-apocalyptic
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5

// Mutex for critical section
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Condition variable for synchronization
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Flag to indicate if all threads have finished
int finished = 0;

// Function to be executed by each thread
void *thread_func(void *arg) {
    // Lock the mutex before entering critical section
    pthread_mutex_lock(&mutex);

    // Wait for all threads to enter critical section
    pthread_cond_wait(&cond, &mutex);

    // Execute thread-specific task
    int thread_id = *(int *)arg;
    printf("Thread %d has entered critical section\n", thread_id);

    // Signal that this thread has finished
    finished++;

    // Unlock the mutex after leaving critical section
    pthread_mutex_unlock(&mutex);

    // Exit the thread
    return NULL;
}

int main() {
    // Create an array of threads
    pthread_t threads[NUM_THREADS];

    // Create each thread and pass its ID as argument
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void *)&i);
    }

    // Wait for all threads to finish
    while (finished < NUM_THREADS) {
        sleep(1);
    }

    // Print a message to indicate that all threads have finished
    printf("All threads have finished\n");

    // Exit the program
    return 0;
}