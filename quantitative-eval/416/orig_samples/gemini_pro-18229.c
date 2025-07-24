//GEMINI-pro DATASET v1.0 Category: Elevator Simulation ; Style: protected
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_FLOORS 10
#define NUM_ELEVATORS 2

typedef struct {
  int floor;
  int direction;
  int passengers;
} Elevator;

Elevator elevators[NUM_ELEVATORS];

pthread_mutex_t elevator_mutex = PTHREAD_MUTEX_INITIALIZER;

void *elevator_thread(void *arg) {
  int elevator_id = *(int *)arg;

  while (1) {
    pthread_mutex_lock(&elevator_mutex);

    // Check if there are any passengers waiting on the current floor
    if (elevators[elevator_id].passengers > 0) {
      // Get the destination floor of the first passenger
      int destination_floor = elevators[elevator_id].passengers;

      // Move the elevator to the destination floor
      elevators[elevator_id].floor = destination_floor;

      // Drop off the passengers
      elevators[elevator_id].passengers = 0;
    } else {
      // Check if there are any passengers waiting on any other floor
      for (int i = 0; i < NUM_FLOORS; i++) {
        if (elevators[elevator_id].floor != i && elevators[elevator_id].passengers == 0) {
          // Move the elevator to the floor with the waiting passengers
          elevators[elevator_id].floor = i;

          // Pick up the passengers
          elevators[elevator_id].passengers = elevators[elevator_id].passengers + 1;
        }
      }
    }

    pthread_mutex_unlock(&elevator_mutex);

    // Sleep for a while
    sleep(1);
  }

  return NULL;
}

int main() {
  // Initialize the elevators
  for (int i = 0; i < NUM_ELEVATORS; i++) {
    elevators[i].floor = 0;
    elevators[i].direction = 1;
    elevators[i].passengers = 0;
  }

  // Create the elevator threads
  pthread_t elevator_threads[NUM_ELEVATORS];
  for (int i = 0; i < NUM_ELEVATORS; i++) {
    pthread_create(&elevator_threads[i], NULL, elevator_thread, &i);
  }

  // Join the elevator threads
  for (int i = 0; i < NUM_ELEVATORS; i++) {
    pthread_join(elevator_threads[i], NULL);
  }

  return 0;
}