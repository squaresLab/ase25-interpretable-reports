//GEMINI-pro DATASET v1.0 Category: Natural Language Date Converter ; Style: multi-threaded
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Define the number of threads to use
#define NUM_THREADS 4

// Define the structure to hold the date information
typedef struct {
  int year;
  int month;
  int day;
} Date;

// Define the mutex to protect the shared data
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Define the condition variable to signal when the shared data is ready
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Define the array of dates to convert
Date dates[] = {
  {2020, 1, 1},
  {2020, 2, 1},
  {2020, 3, 1},
  {2020, 4, 1},
  {2020, 5, 1},
  {2020, 6, 1},
  {2020, 7, 1},
  {2020, 8, 1},
  {2020, 9, 1},
  {2020, 10, 1},
  {2020, 11, 1},
  {2020, 12, 1}
};

// Define the function to convert a date to a string
char *convert_date(Date *date) {
  // Allocate memory for the string
  char *string = malloc(11);

  // Convert the date to a string
  sprintf(string, "%d-%02d-%02d", date->year, date->month, date->day);

  // Return the string
  return string;
}

// Define the function to be executed by each thread
void *thread_function(void *arg) {
  // Get the index of the thread
  int index = *(int *)arg;

  // Lock the mutex
  pthread_mutex_lock(&mutex);

  // Wait for the shared data to be ready
  pthread_cond_wait(&cond, &mutex);

  // Convert the date
  char *string = convert_date(&dates[index]);

  // Unlock the mutex
  pthread_mutex_unlock(&mutex);

  // Print the converted date
  printf("%s\n", string);

  // Free the memory allocated for the string
  free(string);

  // Return NULL
  return NULL;
}

// Define the main function
int main() {
  // Create an array of thread IDs
  pthread_t threads[NUM_THREADS];

  // Create the threads
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, thread_function, (void *)&i);
  }

  // Lock the mutex
  pthread_mutex_lock(&mutex);

  // Signal that the shared data is ready
  pthread_cond_broadcast(&cond);

  // Unlock the mutex
  pthread_mutex_unlock(&mutex);

  // Join the threads
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  // Destroy the mutex and condition variable
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);

  // Return 0
  return 0;
}