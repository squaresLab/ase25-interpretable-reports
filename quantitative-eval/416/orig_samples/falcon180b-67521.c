//Falcon-180B DATASET v1.0 Category: Airport Baggage Handling Simulation ; Style: multi-threaded
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_BAGS 100
#define NUM_THREADS 5

int bags_arrived = 0;
int bags_handled = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *baggage_handler(void *threadid) {
    int tid = *((int *) threadid);
    printf("Baggage Handler %d started\n", tid);

    while (1) {
        pthread_mutex_lock(&mutex);
        if (bags_arrived == NUM_BAGS) {
            printf("Baggage Handler %d finished\n", tid);
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }
}

void *baggage_arrival(void *threadid) {
    int tid = *((int *) threadid);
    printf("Baggage Arrival %d started\n", tid);

    for (int i = 0; i < 20; i++) {
        pthread_mutex_lock(&mutex);
        bags_arrived++;
        printf("Baggage Arrival %d arrived with bag %d\n", tid, i);
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }
}

int main() {
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, baggage_arrival, (void *) &i);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}