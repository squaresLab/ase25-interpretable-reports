//Falcon-180B DATASET v1.0 Category: Tower of Hanoi Problem ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX 100
#define NUM_THREADS 3

int top = -1;
int shared_stack[MAX];
int thread_stack[NUM_THREADS][MAX];
int thread_id[NUM_THREADS];

sem_t mutex;
sem_t empty;
sem_t full;

void init_semaphores() {
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, NUM_THREADS);
    sem_init(&full, 0, 0);
}

void push(int value) {
    sleep(1);
    sem_wait(&empty);
    sem_wait(&mutex);

    shared_stack[++top] = value;

    for (int i = 0; i < NUM_THREADS; i++) {
        if (thread_id[i] == value) {
            thread_stack[i][++thread_id[i]] = value;
            break;
        }
    }

    sem_post(&mutex);
    sleep(2);
    sem_post(&full);
}

void* thread_function(void* arg) {
    int id = *(int*) arg;
    thread_id[id] = id;

    for (int i = 0; i < MAX; i++) {
        push(i);
    }

    return NULL;
}

int main() {
    init_semaphores();

    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_function, (void*) &i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}