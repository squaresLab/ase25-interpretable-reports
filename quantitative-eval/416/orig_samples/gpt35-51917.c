//FormAI DATASET v1.0 Category: Threading Library Implementation ; Style: lively
// Hello there! I'm a Chatbot and I'm excited to show you a unique C Threading Library Implementation!
// In this program, we will create multiple threads and demonstrate their communication using locks and condition variables.
// Let's get started!

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// We will create 5 threads that will print a message to the console, but they will do it in a random order.
const int NUM_THREADS = 5;

// We need a mutex lock to ensure only one thread prints to the console at a time.
pthread_mutex_t lock;

// We also need a condition variable to ensure the threads wait for each other to finish printing.
pthread_cond_t cond_var;

// This is the function that each thread will execute.
// It will lock the mutex, print the message to the console, unlock the mutex, and signal the condition variable.
void* thread_func(void* arg) {
    int thread_num = *(int*)arg;
    pthread_mutex_lock(&lock);
    printf("Thread %d: Hello, world!\n", thread_num);
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&cond_var);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Initialize the mutex lock and condition variable.
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond_var, NULL);

    // Create and start the threads.
    int thread_args[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &thread_args[i]);
    }

    // Wait for the threads to finish printing before exiting the program.
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_mutex_lock(&lock);
        pthread_cond_wait(&cond_var, &lock);
        pthread_mutex_unlock(&lock);
    }

    // Destroy the mutex lock and condition variable.
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond_var);

    return 0;
}