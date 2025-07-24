//Falcon-180B DATASET v1.0 Category: Threading Library Implementation ; Style: surrealist
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Mutex for synchronization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Condition variable for signaling
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Global data shared by threads
int shared_data = 0;

// Function executed by each thread
void *thread_func(void *arg) {
    int id = *(int*) arg;
    printf("Thread %d started\n", id);

    // Lock the mutex before accessing shared data
    pthread_mutex_lock(&mutex);

    // Wait for turn using condition variable
    printf("Thread %d is waiting...\n", id);
    pthread_cond_wait(&cond, &mutex);

    // Update shared data
    shared_data++;
    printf("Thread %d updated shared data to %d\n", id, shared_data);

    // Signal next thread
    pthread_cond_signal(&cond);

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    int num_threads = 5;
    pthread_t threads[num_threads];

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void*) &i);
    }

    // Join threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}