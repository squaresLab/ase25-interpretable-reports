//GEMINI-pro DATASET v1.0 Category: Threading Library Implementation ; Style: multivariable
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Mutex to protect shared data
pthread_mutex_t mutex;

// Condition variable to signal when shared data is ready
pthread_cond_t cond;

// Shared data
int sum = 0;

// Thread function to calculate the sum of an array
void *sum_array(void *arg) {
  int *arr = (int *)arg;
  int size = arr[0];
  int i;

  // Acquire the mutex lock
  pthread_mutex_lock(&mutex);

  // Calculate the sum of the array
  for (i = 1; i <= size; i++) {
    sum += arr[i];
  }

  // Signal that the sum is ready
  pthread_cond_signal(&cond);

  // Release the mutex lock
  pthread_mutex_unlock(&mutex);

  return NULL;
}

int main() {
  int arr[] = {5, 1, 2, 3, 4};
  int size = sizeof(arr) / sizeof(int);
  pthread_t thread;

  // Initialize the mutex and condition variable
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cond, NULL);

  // Create a thread to calculate the sum of the array
  pthread_create(&thread, NULL, sum_array, arr);

  // Wait for the sum to be ready
  pthread_mutex_lock(&mutex);
  pthread_cond_wait(&cond, &mutex);

  // Print the sum of the array
  printf("The sum of the array is: %d\n", sum);

  // Destroy the mutex and condition variable
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);

  return 0;
}