//GEMINI-pro DATASET v1.0 Category: Elevator Simulation ; Style: funny
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// Elevator states
#define IDLE 0
#define UP 1
#define DOWN 2

// Elevator directions
#define UPWARD 1
#define DOWNWARD -1

// Maximum number of floors
#define MAX_FLOORS 10

// Maximum number of people in the elevator
#define MAX_PEOPLE 5

// Global variables
int current_floor = 1;
int destination_floor = 1;
int direction = UPWARD;
int state = IDLE;
int num_people = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Thread function for the elevator
void *elevator_thread(void *arg)
{
    while (1)
    {
        // Acquire the lock
        pthread_mutex_lock(&mutex);

        // Wait for a request
        while (state == IDLE)
        {
            pthread_cond_wait(&cond, &mutex);
        }

        // Set the destination floor
        destination_floor = *(int *)arg;

        // Set the direction
        if (destination_floor > current_floor)
        {
            direction = UPWARD;
        }
        else
        {
            direction = DOWNWARD;
        }

        // Set the state to moving
        state = direction == UPWARD ? UP : DOWN;

        // Release the lock
        pthread_mutex_unlock(&mutex);

        // Move the elevator
        while (current_floor != destination_floor)
        {
            // Simulate the elevator moving
            sleep(1);

            // Update the current floor
            current_floor += direction;
        }

        // Set the state to idle
        pthread_mutex_lock(&mutex);
        state = IDLE;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

// Thread function for the people
void *people_thread(void *arg)
{
    int floor = *(int *)arg;

    // Simulate the person waiting for the elevator
    sleep(rand() % 5);

    // Acquire the lock
    pthread_mutex_lock(&mutex);

    // Add the person to the elevator
    num_people++;

    // If the elevator is idle, wake it up
    if (state == IDLE)
    {
        pthread_cond_signal(&cond);
    }

    // Release the lock
    pthread_mutex_unlock(&mutex);

    // Simulate the person riding the elevator
    sleep(rand() % 5);

    // Acquire the lock
    pthread_mutex_lock(&mutex);

    // Remove the person from the elevator
    num_people--;

    // Release the lock
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main()
{
    // Create the elevator thread
    pthread_t elevator;
    pthread_create(&elevator, NULL, elevator_thread, NULL);

    // Create a random number of people threads
    int num_people = rand() % 10;
    pthread_t people[num_people];
    for (int i = 0; i < num_people; i++)
    {
        int floor = rand() % MAX_FLOORS + 1;
        pthread_create(&people[i], NULL, people_thread, &floor);
    }

    // Join the threads
    pthread_join(elevator, NULL);
    for (int i = 0; i < num_people; i++)
    {
        pthread_join(people[i], NULL);
    }

    return 0;
}