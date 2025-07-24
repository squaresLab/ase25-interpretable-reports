//GEMINI-pro DATASET v1.0 Category: Prime Number Generator ; Style: multi-threaded
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Define the maximum number of primes to generate
#define MAX_PRIMES 1000000

// Define the number of threads to use
#define NUM_THREADS 4

// Create a mutex to protect the shared variable
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Create a shared variable to store the current prime number
int current_prime = 2;

// Create a thread function that generates prime numbers
void *generate_primes(void *arg) {
  // Get the thread number
  int thread_num = *(int *)arg;

  // Generate prime numbers until the current prime number is greater than the maximum prime number
  while (current_prime <= MAX_PRIMES) {
    // Lock the mutex
    pthread_mutex_lock(&mutex);

    // Check if the current prime number is prime
    int is_prime = 1;
    for (int i = 2; i <= current_prime / 2; i++) {
      if (current_prime % i == 0) {
        is_prime = 0;
        break;
      }
    }

    // If the current prime number is prime, print it
    if (is_prime) {
      printf("%d ", current_prime);
    }

    // Increment the current prime number
    current_prime++;

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);
  }

  // Exit the thread
  pthread_exit(NULL);
}

// Create a main function that creates and runs the threads
int main() {
  // Create an array of thread IDs
  pthread_t threads[NUM_THREADS];

  // Create the threads
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, generate_primes, &i);
  }

  // Wait for the threads to finish
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  // Destroy the mutex
  pthread_mutex_destroy(&mutex);

  // Exit the program
  return 0;
}