//FormAI DATASET v1.0 Category: Threading Library Implementation ; Style: brave
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5

void *thread_function(void *arg);

int main()
{
    int res;
    pthread_t threads[NUM_THREADS];

    for(int i = 0; i < NUM_THREADS; i++)
    {
        printf("Creating thread %d\n", i);
        res = pthread_create(&threads[i], NULL, thread_function, (void *)&i);
        if (res != 0)
        {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
    }

    printf("All threads created.\n");

    for (int i = 0; i < NUM_THREADS; i++)
    {
        res = pthread_join(threads[i], NULL);
        if (res != 0)
        {
            perror("Thread join failed");
            exit(EXIT_FAILURE);
        }
        printf("Thread %d joined\n", i);
    }

    printf("All threads joined. Program exiting.\n");
    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg)
{
    int index = *((int *)arg);
    int random_time = rand() % 5 + 1;

    printf("Thread %d sleeping for %d second(s)...\n", index, random_time);
    sleep(random_time);
    printf("Thread %d woke up after %d second(s).\n", index, random_time);

    pthread_exit(NULL);
}