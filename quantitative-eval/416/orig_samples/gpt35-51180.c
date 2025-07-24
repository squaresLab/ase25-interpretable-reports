//FormAI DATASET v1.0 Category: Task Scheduler ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>

#define TASK_NAME_MAX_LENGTH 50
#define MAX_TASKS 100
#define MAX_THREAD_POOL_SIZE 10

typedef struct {
    char task_name[TASK_NAME_MAX_LENGTH];
    int priority;
    int frequency; 
    char task_location[50];
    bool is_running;
} Task;

Task task_queue[MAX_TASKS];
int task_queue_length = 0;
pthread_mutex_t task_queue_mutex;

void add_task_to_queue(Task task) {
    pthread_mutex_lock(&task_queue_mutex);

    task_queue[task_queue_length++] = task;

    pthread_mutex_unlock(&task_queue_mutex);
}

Task get_highest_priority_task() {
    pthread_mutex_lock(&task_queue_mutex);

    Task highest_priority_task = task_queue[0];
    int highest_priority_index = 0;

    for(int i=1; i<task_queue_length; i++) {
        if(task_queue[i].priority > highest_priority_task.priority) {
            highest_priority_task = task_queue[i];
            highest_priority_index = i;
        }
    }

    for(int i=highest_priority_index; i<task_queue_length-1; i++) {
        task_queue[i] = task_queue[i+1];
    }

    task_queue_length--;

    pthread_mutex_unlock(&task_queue_mutex);

    return highest_priority_task;
}

void* run_task(void* task) {
    Task* current_task = (Task*)task;

    printf("Executing task: %s\n", current_task->task_name);
    system(current_task->task_location);

    current_task->is_running = false;

    return NULL;
}

void dispatch_tasks(int thread_pool_size) {
    pthread_t thread_pool[MAX_THREAD_POOL_SIZE];
    int threads_in_use = 0;

    while(1) {
        if(threads_in_use == thread_pool_size || task_queue_length == 0) {
            continue;
        }

        Task highest_priority_task = get_highest_priority_task();

        pthread_create(&thread_pool[threads_in_use++], NULL, run_task, (void*)&highest_priority_task);

        while(threads_in_use != 0 && pthread_tryjoin_np(thread_pool[threads_in_use-1], NULL) == 0) {
            threads_in_use--;
        }
    }
}

int main() {
    srand(time(NULL));

    pthread_mutex_init(&task_queue_mutex, NULL);

    Task task1 = {"task1", 2, 5, "echo 'This is task 1'", false};
    Task task2 = {"task2", 3, 10, "mkdir task2_folder", false};
    Task task3 = {"task3", 1, 3, "ls", false};
    Task task4 = {"task4", 4, 2, "touch task4_file", false};

    add_task_to_queue(task1);
    add_task_to_queue(task2);
    add_task_to_queue(task3);
    add_task_to_queue(task4);

    dispatch_tasks(4);

    pthread_mutex_destroy(&task_queue_mutex);

    return 0;
}