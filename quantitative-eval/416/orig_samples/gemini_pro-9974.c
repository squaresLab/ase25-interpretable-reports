//GEMINI-pro DATASET v1.0 Category: Drone Remote Control ; Style: multi-threaded
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#define NUM_DRONES 4

// Shared data variables
int drone_altitudes[NUM_DRONES];
pthread_mutex_t altitude_lock;

// Drone control thread
void *drone_control(void *arg) {
    int drone_id = *(int *)arg;
    while (1) {
        // Randomly adjust drone altitude
        pthread_mutex_lock(&altitude_lock);
        drone_altitudes[drone_id] += rand() % 10 - 5;
        printf("Drone %d: Altitude: %d\n", drone_id, drone_altitudes[drone_id]);
        pthread_mutex_unlock(&altitude_lock);
        
        // Sleep for a while
        sleep(1);
    }
    return NULL;
}

int main() {
    // Initialize shared data variables
    for (int i = 0; i < NUM_DRONES; i++) {
        drone_altitudes[i] = 0;
    }
    pthread_mutex_init(&altitude_lock, NULL);

    // Create drone control threads
    pthread_t drone_threads[NUM_DRONES];
    for (int i = 0; i < NUM_DRONES; i++) {
        pthread_create(&drone_threads[i], NULL, drone_control, (void *)&i);
    }

    // Wait for drone control threads to finish
    for (int i = 0; i < NUM_DRONES; i++) {
        pthread_join(drone_threads[i], NULL);
    }

    // Destroy shared data variables
    pthread_mutex_destroy(&altitude_lock);

    return 0;
}