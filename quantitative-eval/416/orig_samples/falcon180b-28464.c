//Falcon-180B DATASET v1.0 Category: Threading Library Implementation ; Style: inquisitive
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Mutex for critical section
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Condition variable for synchronization
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Number of threads
#define NUM_THREADS 5

// Function to be executed by each thread
void* thread_func(void* arg) {
    int id = *(int*) arg;

    printf("Thread %d started\n", id);

    // Lock the mutex before entering the critical section
    pthread_mutex_lock(&mutex);

    // Notify the main thread that this thread has entered the critical section
    pthread_cond_signal(&cond);

    // Sleep for a random amount of time
    sleep(rand() % 3);

    // Unlock the mutex after exiting the critical section
    pthread_mutex_unlock(&mutex);

    printf("Thread %d exited\n", id);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS] = {1, 2, 3, 4, 5};

    // Create the threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, &ids[i]);
    }

    // Wait for all threads to enter the critical section
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_cond_wait(&cond, &mutex);
        printf("All threads have entered the critical section\n");
    }

    // Unlock the mutex and exit the program
    pthread_mutex_unlock(&mutex);
    return 0;
}