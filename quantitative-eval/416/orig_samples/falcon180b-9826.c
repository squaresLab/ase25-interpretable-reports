//Falcon-180B DATASET v1.0 Category: System event logger ; Style: imaginative
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define MAX_EVENTS 1000
#define MAX_THREADS 10

typedef struct event_s {
    time_t timestamp;
    char message[256];
} event_t;

typedef struct thread_data_s {
    int thread_id;
    FILE *log_file;
} thread_data_t;

event_t event_buffer[MAX_EVENTS];
int event_head = 0;
int event_tail = 0;
pthread_mutex_t event_mutex = PTHREAD_MUTEX_INITIALIZER;

void log_event(const char *message) {
    event_t event;
    time(&event.timestamp);
    strncpy(event.message, message, 255);
    event.message[255] = '\0';

    pthread_mutex_lock(&event_mutex);
    if ((event_tail + 1) % MAX_EVENTS == event_head) {
        printf("Event buffer full, dropping event\n");
    } else {
        event_tail = (event_tail + 1) % MAX_EVENTS;
        event_buffer[event_tail] = event;
    }
    pthread_mutex_unlock(&event_mutex);
}

void *log_to_file(void *arg) {
    thread_data_t *data = (thread_data_t *) arg;
    FILE *log_file = data->log_file;

    while (1) {
        pthread_mutex_lock(&event_mutex);
        if (event_head == event_tail) {
            pthread_mutex_unlock(&event_mutex);
            sleep(1);
            continue;
        }

        fprintf(log_file, "Thread %d logging event: %s\n", data->thread_id, event_buffer[event_head].message);
        fflush(log_file);

        event_head = (event_head + 1) % MAX_EVENTS;
        pthread_mutex_unlock(&event_mutex);

        sleep(1);
    }

    return NULL;
}

int main() {
    int i;
    pthread_t threads[MAX_THREADS];

    for (i = 0; i < MAX_THREADS; i++) {
        thread_data_t data;
        data.thread_id = i;
        data.log_file = fopen("logfile.txt", "a");

        pthread_create(&threads[i], NULL, log_to_file, &data);
    }

    while (1) {
        sleep(1);
        log_event("Hello world!");
    }

    return 0;
}