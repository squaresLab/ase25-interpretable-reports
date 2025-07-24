//Falcon-180B DATASET v1.0 Category: Smart home light control ; Style: decentralized
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>

#define MAX_CLIENTS 10
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int clients[MAX_CLIENTS];
sem_t mutex;

void init_semaphore() {
    sem_init(&mutex, 0, 1);
}

void lock_semaphore() {
    sem_wait(&mutex);
}

void unlock_semaphore() {
    sem_post(&mutex);
}

void *handle_client(void *client_socket) {
    int client_socket_fd = *((int *) client_socket);
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket_fd, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        printf("Client %d sent: %s\n", client_socket_fd, buffer);
    }

    close(client_socket_fd);
}

int main() {
    init_semaphore();

    int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(server_socket_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));

    listen(server_socket_fd, MAX_CLIENTS);

    while (1) {
        int client_socket_fd = accept(server_socket_fd, NULL, NULL);

        if (client_socket_fd < 0) {
            printf("Error accepting client\n");
            continue;
        }

        lock_semaphore();

        int i = 0;
        while (clients[i]!= 0) {
            i++;
        }

        clients[i] = client_socket_fd;
        printf("Client %d connected\n", client_socket_fd);

        unlock_semaphore();

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, &client_socket_fd);
    }

    close(server_socket_fd);
    sem_destroy(&mutex);

    return 0;
}