//GEMINI-pro DATASET v1.0 Category: Threading Library Implementation ; Style: scientific
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Define the number of threads
#define NUM_THREADS 4

// Define the global variables
int shared_variable = 0;
pthread_mutex_t lock;

// Define the thread function
void *thread_function(void *arg) {
  // Get the thread ID
  int thread_id = *(int *)arg;

  // Perform the task
  for (int i = 0; i < 100000; i++) {
    // Lock the mutex
    pthread_mutex_lock(&lock);

    // Increment the shared variable
    shared_variable++;

    // Unlock the mutex
    pthread_mutex_unlock(&lock);
  }

  // Return NULL
  return NULL;
}

// Main function
int main() {
  // Initialize the mutex
  pthread_mutex_init(&lock, NULL);

  // Create the threads
  pthread_t threads[NUM_THREADS];
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, thread_function, (void *)&i);
  }

  // Join the threads
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  // Print the shared variable
  printf("The shared variable is: %d\n", shared_variable);

  // Destroy the mutex
  pthread_mutex_destroy(&lock);

  // Return 0
  return 0;
}