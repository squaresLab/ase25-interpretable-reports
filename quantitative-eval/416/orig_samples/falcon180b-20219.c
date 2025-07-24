//Falcon-180B DATASET v1.0 Category: Threading Library Implementation ; Style: complex
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Condition variable
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Shared data
int shared_data = 0;

void* thread_func(void* arg) {
    int id = *((int*) arg);

    printf("Thread %d started\n", id);

    // Lock the mutex
    pthread_mutex_lock(&mutex);

    // Wait for the condition variable to be signaled
    pthread_cond_wait(&cond, &mutex);

    // Increment the shared data
    shared_data++;

    // Signal the condition variable
    pthread_cond_signal(&cond);

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);

    printf("Thread %d finished\n", id);

    return NULL;
}

int main() {
    int num_threads = 5;
    pthread_t threads[num_threads];

    // Create the threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void*) &i);
    }

    // Join the threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Shared data: %d\n", shared_data);

    return 0;
}