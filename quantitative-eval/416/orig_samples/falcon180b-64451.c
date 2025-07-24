//Falcon-180B DATASET v1.0 Category: Graph representation ; Style: asynchronous
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_THREADS 10
#define MAX_QUEUE 100

typedef struct {
    int id;
    pthread_t thread_id;
    int is_running;
} thread_t;

typedef struct {
    int front, rear;
    thread_t threads[MAX_THREADS];
} queue_t;

queue_t queue;

void *worker(void *arg) {
    int id = *(int *) arg;
    printf("Thread %d is running\n", id);
    sleep(1);
    printf("Thread %d is done\n", id);
    return NULL;
}

void create_thread(int id) {
    thread_t *thread = &queue.threads[id];
    pthread_create(&thread->thread_id, NULL, worker, &id);
    thread->is_running = 1;
}

void start_queue() {
    int i;
    for (i = 0; i < MAX_THREADS; i++) {
        create_thread(i);
    }
}

void stop_queue() {
    int i;
    for (i = 0; i < MAX_THREADS; i++) {
        thread_t *thread = &queue.threads[i];
        if (thread->is_running) {
            pthread_cancel(thread->thread_id);
            thread->is_running = 0;
        }
    }
}

int main() {
    int i;
    for (i = 0; i < MAX_THREADS; i++) {
        queue.threads[i].id = i;
        queue.threads[i].is_running = 0;
    }
    start_queue();
    sleep(5);
    stop_queue();
    return 0;
}