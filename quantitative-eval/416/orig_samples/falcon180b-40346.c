//Falcon-180B DATASET v1.0 Category: CPU Scheduling Algorithms ; Style: grateful
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sched.h>

#define NUM_THREADS 5
#define STACK_SIZE 4096

// Mutex for critical section
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Semaphore for synchronization
sem_t sem;

// Thread IDs
pthread_t threads[NUM_THREADS];

// Thread function
void *runner(void *arg) {
    int id = *(int *) arg;
    int priority = (id * 10) + 1;

    printf("Thread %d started with priority %d\n", id, priority);

    // Yield the CPU for a certain amount of time slices
    for (int i = 0; i < 10; i++) {
        sched_yield();
    }

    printf("Thread %d finished\n", id);
    return NULL;
}

int main() {
    // Initialize semaphore
    sem_init(&sem, 0, 0);

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, runner, (void *) &i);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}