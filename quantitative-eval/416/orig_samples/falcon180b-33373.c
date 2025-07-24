//Falcon-180B DATASET v1.0 Category: Task Scheduler ; Style: careful
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Global variable to keep track of the number of tasks
int num_tasks = 0;

// Mutex for thread-safe access to the num_tasks variable
pthread_mutex_t num_tasks_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to create a new task
void* task_func(void* arg) {
    int task_id = *(int*) arg;
    printf("Starting task %d\n", task_id);
    sleep(1);
    printf("Finished task %d\n", task_id);
    return NULL;
}

// Function to add a new task to the scheduler
void add_task() {
    pthread_mutex_lock(&num_tasks_mutex);
    num_tasks++;
    pthread_mutex_unlock(&num_tasks_mutex);

    int task_id = num_tasks;
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, task_func, &task_id);
    printf("Added task %d\n", task_id);
}

// Function to remove a task from the scheduler
void remove_task(int task_id) {
    pthread_mutex_lock(&num_tasks_mutex);
    num_tasks--;
    pthread_mutex_unlock(&num_tasks_mutex);

    printf("Removed task %d\n", task_id);
}

// Function to print the current state of the scheduler
void print_state() {
    pthread_mutex_lock(&num_tasks_mutex);
    printf("Current number of tasks: %d\n", num_tasks);
    pthread_mutex_unlock(&num_tasks_mutex);
}

int main() {
    add_task();
    add_task();
    add_task();

    sleep(2);

    remove_task(1);

    print_state();

    return 0;
}