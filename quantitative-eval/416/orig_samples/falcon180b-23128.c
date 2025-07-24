//Falcon-180B DATASET v1.0 Category: Threading Library Implementation ; Style: Cryptic
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Define the number of threads
#define NUM_THREADS 5

// Define the size of the shared buffer
#define BUFFER_SIZE 10

// Define the mutex for the shared buffer
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;

// Define the shared buffer
int buffer[BUFFER_SIZE];

// Define the thread IDs
pthread_t threads[NUM_THREADS];

// Define the thread function
void *thread_function(void *arg)
{
    // Get the thread ID
    int thread_id = *(int *) arg;

    // Print a message from the thread
    printf("Thread %d started.\n", thread_id);

    // Wait for a random amount of time
    int delay = rand() % 5;
    usleep(delay * 100000);

    // Add a random number to the shared buffer
    int value = rand() % 100;
    pthread_mutex_lock(&buffer_mutex);
    buffer[thread_id] = value;
    pthread_mutex_unlock(&buffer_mutex);

    // Print a message from the thread
    printf("Thread %d added %d to the buffer.\n", thread_id, value);

    // Exit the thread
    return NULL;
}

int main()
{
    // Initialize the random number generator
    srand(time(NULL));

    // Create the threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        // Create the thread
        pthread_create(&threads[i], NULL, thread_function, (void *) &i);
    }

    // Wait for the threads to finish
    for (int i = 0; i < NUM_THREADS; i++)
    {
        // Join the thread
        pthread_join(threads[i], NULL);
    }

    // Print the contents of the shared buffer
    printf("Shared buffer:\n");
    for (int i = 0; i < NUM_THREADS; i++)
    {
        printf("%d: %d\n", i, buffer[i]);
    }

    return 0;
}