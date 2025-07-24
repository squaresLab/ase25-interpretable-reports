//GEMINI-pro DATASET v1.0 Category: Traffic Light Controller ; Style: protected
// Traffic Light Controller in C

#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

// Define the traffic light states
#define RED 0
#define YELLOW 1
#define GREEN 2

// Define the number of traffic lights
#define NUM_TRAFFIC_LIGHTS 3

// Define the traffic light colors
const char *traffic_light_colors[] = {"RED", "YELLOW", "GREEN"};

// Create a semaphore for each traffic light
sem_t traffic_lights[NUM_TRAFFIC_LIGHTS];

// Create a mutex to protect the traffic lights
pthread_mutex_t traffic_lights_mutex = PTHREAD_MUTEX_INITIALIZER;

// Create a thread for each traffic light
pthread_t traffic_light_threads[NUM_TRAFFIC_LIGHTS];

// Function to control a traffic light
void *traffic_light_controller(void *arg)
{
    int traffic_light_id = *(int *)arg;
    while (true)
    {
        // Acquire the traffic lights mutex
        pthread_mutex_lock(&traffic_lights_mutex);

        // Set the traffic light to red
        sem_wait(&traffic_lights[traffic_light_id]);

        // Sleep for a while
        sleep(1);

        // Set the traffic light to yellow
        sem_post(&traffic_lights[traffic_light_id]);

        // Sleep for a while
        sleep(1);

        // Set the traffic light to green
        sem_post(&traffic_lights[traffic_light_id]);

        // Sleep for a while
        sleep(1);

        // Release the traffic lights mutex
        pthread_mutex_unlock(&traffic_lights_mutex);
    }
    return NULL;
}

// Main function
int main()
{
    // Initialize the semaphores
    for (int i = 0; i < NUM_TRAFFIC_LIGHTS; i++)
    {
        sem_init(&traffic_lights[i], 0, 0);
    }

    // Create the traffic light threads
    for (int i = 0; i < NUM_TRAFFIC_LIGHTS; i++)
    {
        pthread_create(&traffic_light_threads[i], NULL, traffic_light_controller, &i);
    }

    // Join the traffic light threads
    for (int i = 0; i < NUM_TRAFFIC_LIGHTS; i++)
    {
        pthread_join(traffic_light_threads[i], NULL);
    }

    // Destroy the semaphores
    for (int i = 0; i < NUM_TRAFFIC_LIGHTS; i++)
    {
        sem_destroy(&traffic_lights[i]);
    }

    // Destroy the traffic lights mutex
    pthread_mutex_destroy(&traffic_lights_mutex);

    return 0;
}