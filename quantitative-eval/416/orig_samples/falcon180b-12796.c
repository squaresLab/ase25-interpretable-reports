//Falcon-180B DATASET v1.0 Category: Task Scheduler ; Style: peaceful
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

// Function prototypes
void *run_task(void *arg);
void schedule_task(int task_id, int interval, int duration);

int main() {
    int num_tasks = 5;
    int task_ids[num_tasks];
    int intervals[num_tasks];
    int durations[num_tasks];

    // Initialize task IDs, intervals, and durations
    for (int i = 0; i < num_tasks; i++) {
        task_ids[i] = i + 1;
        intervals[i] = 1000; // 1 second
        durations[i] = 5000; // 5 seconds
    }

    // Schedule tasks
    for (int i = 0; i < num_tasks; i++) {
        schedule_task(task_ids[i], intervals[i], durations[i]);
    }

    // Join scheduled tasks
    for (int i = 0; i < num_tasks; i++) {
        pthread_join(task_ids[i], NULL);
    }

    return 0;
}

void *run_task(void *arg) {
    int task_id = *((int *) arg);
    int interval = 1000; // 1 second
    int duration = 5000; // 5 seconds

    while (duration > 0) {
        printf("Task %d is running...\n", task_id);
        sleep(interval);
        duration -= interval;
    }

    printf("Task %d has completed.\n", task_id);
    return NULL;
}

void schedule_task(int task_id, int interval, int duration) {
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, run_task, &task_id);
    sleep(interval);
    pthread_cancel(thread_id);
}