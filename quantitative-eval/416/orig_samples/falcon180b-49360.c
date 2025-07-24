//Falcon-180B DATASET v1.0 Category: Intrusion detection system ; Style: Linus Torvalds
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define BUFFER_SIZE 1024
#define MAX_THREADS 10

typedef struct {
    int thread_id;
    int fd;
} thread_data_t;

void *monitor_thread(void *param) {
    thread_data_t *data = (thread_data_t *) param;
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(data->fd, buffer, BUFFER_SIZE);
        if (bytes_read <= 0) {
            break;
        }
        printf("Thread %d: %s\n", data->thread_id, buffer);
    }
    return NULL;
}

int main() {
    int threads_created = 0;
    pthread_t threads[MAX_THREADS];
    while (threads_created < MAX_THREADS) {
        int fd = open("/dev/urandom", O_RDONLY);
        if (fd == -1) {
            printf("Failed to open /dev/urandom\n");
            break;
        }
        thread_data_t data;
        data.thread_id = threads_created + 1;
        data.fd = fd;
        int result = pthread_create(&threads[threads_created], NULL, monitor_thread, &data);
        if (result!= 0) {
            printf("Failed to create thread\n");
            break;
        }
        threads_created++;
    }
    for (int i = 0; i < threads_created; i++) {
        pthread_join(threads[i], NULL);
    }
    return 0;
}