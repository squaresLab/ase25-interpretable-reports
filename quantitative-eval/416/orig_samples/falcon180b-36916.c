//Falcon-180B DATASET v1.0 Category: Task Scheduler ; Style: Ken Thompson
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

// Task structure
typedef struct {
    void (*function)(void *param);
    void *param;
    struct task *next;
} task_t;

// Task Scheduler structure
typedef struct {
    task_t *head;
    pthread_t scheduler_thread;
} scheduler_t;

// Function prototypes
void add_task(scheduler_t *scheduler, void (*function)(void *param), void *param);
void start_scheduler(scheduler_t *scheduler);
void *scheduler_thread(void *arg);

// Add a task to the scheduler
void add_task(scheduler_t *scheduler, void (*function)(void *param), void *param) {
    task_t *task = malloc(sizeof(task_t));
    task->function = function;
    task->param = param;
    task->next = NULL;

    if (scheduler->head == NULL) {
        scheduler->head = task;
    } else {
        task_t *current = scheduler->head;
        while (current->next!= NULL) {
            current = current->next;
        }
        current->next = task;
    }
}

// Start the scheduler
void start_scheduler(scheduler_t *scheduler) {
    pthread_create(&scheduler->scheduler_thread, NULL, scheduler_thread, scheduler);
}

// Scheduler thread function
void *scheduler_thread(void *arg) {
    scheduler_t *scheduler = arg;
    task_t *task = scheduler->head;

    while (task!= NULL) {
        task->function(task->param);
        task = task->next;
    }

    return NULL;
}

// Example task function
void example_task(void *param) {
    printf("Hello, world!\n");
}

int main() {
    scheduler_t scheduler;
    scheduler.head = NULL;
    scheduler.scheduler_thread = 0;

    add_task(&scheduler, example_task, NULL);

    start_scheduler(&scheduler);

    return 0;
}