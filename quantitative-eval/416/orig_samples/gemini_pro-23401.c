//GEMINI-pro DATASET v1.0 Category: Traffic Light Controller ; Style: interoperable
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_LIGHTS 3

// Define the traffic light states
enum light_state {
  RED,
  YELLOW,
  GREEN
};

// Define the traffic light colors
enum light_color {
  RED_COLOR,
  YELLOW_COLOR,
  GREEN_COLOR
};

// Create a mutex to protect the shared data
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Create a condition variable to signal when the light state changes
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Create an array of traffic lights
struct traffic_light {
  enum light_state state;
  enum light_color color;
} lights[NUM_LIGHTS];

// Create a thread for each traffic light
pthread_t threads[NUM_LIGHTS];

// Function to change the state of a traffic light
void *change_light_state(void *arg) {
  // Get the index of the traffic light
  int index = *(int *)arg;

  // Loop forever
  while (1) {
    // Lock the mutex
    pthread_mutex_lock(&mutex);

    // Wait for the condition variable to be signaled
    pthread_cond_wait(&cond, &mutex);

    // Change the state of the traffic light
    switch (lights[index].state) {
      case RED:
        lights[index].state = GREEN;
        lights[index].color = GREEN_COLOR;
        break;
      case YELLOW:
        lights[index].state = RED;
        lights[index].color = RED_COLOR;
        break;
      case GREEN:
        lights[index].state = YELLOW;
        lights[index].color = YELLOW_COLOR;
        break;
    }

    // Unlock the mutex
    pthread_mutex_unlock(&mutex);

    // Sleep for 1 second
    sleep(1);
  }

  return NULL;
}

int main() {
  // Initialize the traffic lights
  for (int i = 0; i < NUM_LIGHTS; i++) {
    lights[i].state = RED;
    lights[i].color = RED_COLOR;
  }

  // Create the threads
  for (int i = 0; i < NUM_LIGHTS; i++) {
    pthread_create(&threads[i], NULL, change_light_state, &i);
  }

  // Join the threads
  for (int i = 0; i < NUM_LIGHTS; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}