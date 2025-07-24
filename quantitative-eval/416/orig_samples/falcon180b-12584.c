//Falcon-180B DATASET v1.0 Category: CPU Scheduling Algorithms ; Style: asynchronous
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Define the maximum number of threads
#define MAX_THREADS 10

// Define the thread structure
typedef struct {
    int id;
    int priority;
    int burst_time;
    int waiting_time;
    int turnaround_time;
} Thread;

// Define the ready queue
Thread ready_queue[MAX_THREADS];

// Define the number of threads currently in the system
int num_threads = 0;

// Define the current time
int current_time = 0;

// Define the time quantum
int time_quantum = 2;

// Define the total number of time quanta
int total_time_quanta = 0;

// Define the function to execute a thread
void* execute_thread(void* arg) {
    // Get the thread ID
    int thread_id = *(int*) arg;

    // Calculate the burst time
    int burst_time = ready_queue[thread_id].burst_time;

    // Calculate the waiting time
    int waiting_time = 0;

    // Calculate the turnaround time
    int turnaround_time = 0;

    // Execute the thread
    printf("Thread %d is executing...\n", thread_id);
    sleep(burst_time / time_quantum);

    // Update the waiting time
    waiting_time = burst_time - turnaround_time;

    // Update the turnaround time
    turnaround_time = burst_time + waiting_time;

    // Print the results
    printf("Thread %d finished execution.\n", thread_id);
    printf("Waiting time: %d\n", waiting_time);
    printf("Turnaround time: %d\n", turnaround_time);

    // Exit the thread
    pthread_exit(0);
}

// Define the function to add a thread to the ready queue
void add_thread(int id, int priority, int burst_time) {
    // Check if the ready queue is full
    if (num_threads >= MAX_THREADS) {
        printf("Error: Ready queue is full.\n");
        return;
    }

    // Add the thread to the ready queue
    ready_queue[num_threads].id = id;
    ready_queue[num_threads].priority = priority;
    ready_queue[num_threads].burst_time = burst_time;

    // Increment the number of threads
    num_threads++;
}

// Define the function to execute the threads
void execute_threads() {
    // Initialize the current time
    current_time = 0;

    // Initialize the total number of time quanta
    total_time_quanta = 0;

    // Execute each thread
    for (int i = 0; i < num_threads; i++) {
        // Calculate the time quantum for the current thread
        int time_quanta = ready_queue[i].burst_time / time_quantum;

        // Update the total number of time quanta
        total_time_quanta += time_quanta;

        // Execute the thread
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, execute_thread, (void*) &i);

        // Update the current time
        current_time += time_quanta;
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(NULL, NULL);
    }
}

// Define the main function
int main() {
    // Add threads to the ready queue
    add_thread(1, 1, 5);
    add_thread(2, 2, 3);
    add_thread(3, 3, 8);

    // Execute the threads
    execute_threads();

    // Exit the program
    return 0;
}