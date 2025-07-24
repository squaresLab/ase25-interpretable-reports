//GEMINI-pro DATASET v1.0 Category: Traffic Light Controller ; Style: beginner-friendly
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// Traffic light colors
#define RED 0
#define YELLOW 1
#define GREEN 2

// Road directions
#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

// Traffic light state
typedef struct {
    int color;
    int next_color;
    int timer;
} traffic_light;

// Traffic light controller
typedef struct {
    traffic_light lights[4];
    pthread_t threads[4];
} traffic_light_controller;

// Initialize a traffic light controller
void init_traffic_light_controller(traffic_light_controller *controller) {
    for (int i = 0; i < 4; i++) {
        controller->lights[i].color = RED;
        controller->lights[i].next_color = GREEN;
        controller->lights[i].timer = 0;
    }
}

// Change the color of a traffic light
void change_traffic_light_color(traffic_light *light, int color) {
    light->color = color;
    light->timer = 0;
}

// Thread function for a traffic light
void *traffic_light_thread(void *arg) {
    traffic_light *light = (traffic_light *)arg;

    while (1) {
        // Increment the timer
        light->timer++;

        // Change the color of the traffic light if the timer has expired
        if (light->timer >= 5) {
            change_traffic_light_color(light, light->next_color);
            // Change the next color
            light->next_color = (light->next_color + 1) % 3;
        }

        // Sleep for 1 second
        sleep(1);
    }

    return NULL;
}

// Main function
int main() {
    // Initialize the traffic light controller
    traffic_light_controller controller;
    init_traffic_light_controller(&controller);

    // Create the threads for the traffic lights
    for (int i = 0; i < 4; i++) {
        pthread_create(&controller.threads[i], NULL, traffic_light_thread, &controller.lights[i]);
    }

    // Join the threads
    for (int i = 0; i < 4; i++) {
        pthread_join(controller.threads[i], NULL);
    }

    return 0;
}