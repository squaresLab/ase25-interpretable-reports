//GEMINI-pro DATASET v1.0 Category: Traffic Light Controller ; Style: all-encompassing
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Define the traffic light states
#define RED 0
#define YELLOW 1
#define GREEN 2

// Define the number of traffic lights
#define NUM_LIGHTS 3

// Create a mutex to protect the shared data
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Create a condition variable to signal when the traffic light has changed
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Create an array to store the current state of each traffic light
int light_states[NUM_LIGHTS];

// Create a thread for each traffic light
pthread_t threads[NUM_LIGHTS];

// Function to simulate a traffic light
void *traffic_light(void *arg) {
    int light_num = *(int *)arg;

    while (1) {
        // Lock the mutex
        pthread_mutex_lock(&mutex);

        // Wait until the traffic light is green
        while (light_states[light_num] != GREEN) {
            pthread_cond_wait(&cond, &mutex);
        }

        // Unlock the mutex
        pthread_mutex_unlock(&mutex);

        // Display the green light
        printf("Traffic light %d is green\n", light_num);

        // Sleep for 5 seconds
        sleep(5);

        // Lock the mutex
        pthread_mutex_lock(&mutex);

        // Change the traffic light to yellow
        light_states[light_num] = YELLOW;

        // Signal that the traffic light has changed
        pthread_cond_signal(&cond);

        // Unlock the mutex
        pthread_mutex_unlock(&mutex);

        // Display the yellow light
        printf("Traffic light %d is yellow\n", light_num);

        // Sleep for 1 second
        sleep(1);

        // Lock the mutex
        pthread_mutex_lock(&mutex);

        // Change the traffic light to red
        light_states[light_num] = RED;

        // Signal that the traffic light has changed
        pthread_cond_signal(&cond);

        // Unlock the mutex
        pthread_mutex_unlock(&mutex);

        // Display the red light
        printf("Traffic light %d is red\n", light_num);

        // Sleep for 5 seconds
        sleep(5);
    }

    return NULL;
}

int main() {
    // Initialize the traffic light states
    for (int i = 0; i < NUM_LIGHTS; i++) {
        light_states[i] = RED;
    }

    // Create the traffic light threads
    for (int i = 0; i < NUM_LIGHTS; i++) {
        pthread_create(&threads[i], NULL, traffic_light, (void *)&i);
    }

    // Join the traffic light threads
    for (int i = 0; i < NUM_LIGHTS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}