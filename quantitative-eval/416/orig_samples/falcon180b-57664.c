//Falcon-180B DATASET v1.0 Category: Port Scanner ; Style: multiplayer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_THREADS 100
#define MAX_PORTS 65535
#define BUFFER_SIZE 1024

int ports[MAX_PORTS];
int num_ports = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *scan_ports(void *arg) {
    int thread_num = *(int *) arg;
    int start_port = thread_num * (MAX_PORTS / MAX_THREADS) + 1;
    int end_port = (thread_num == MAX_THREADS - 1)? MAX_PORTS : start_port + (MAX_PORTS / MAX_THREADS) - 1;

    for (int i = start_port; i <= end_port; i++) {
        pthread_mutex_lock(&mutex);
        if (num_ports >= MAX_PORTS) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        ports[num_ports++] = i;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main() {
    int num_threads = 4;
    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, scan_ports, (void *) &i);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Found %d open ports:\n", num_ports);
    for (int i = 0; i < num_ports; i++) {
        printf("%d\n", ports[i]);
    }

    return 0;
}