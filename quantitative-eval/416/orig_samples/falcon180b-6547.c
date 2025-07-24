//Falcon-180B DATASET v1.0 Category: Appointment Scheduler ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BACKLOG 10
#define MAX_CLIENTS 10
#define MAX_REQUEST_LENGTH 1024
#define SEMAPHORE_VALUE 1

int g_client_count = 0;
int g_client_sockets[MAX_CLIENTS];
sem_t g_client_sem;

// Function to initialize the semaphore
void init_semaphore() {
    sem_init(&g_client_sem, 0, SEMAPHORE_VALUE);
}

// Function to lock the semaphore
void lock_semaphore() {
    sem_wait(&g_client_sem);
}

// Function to unlock the semaphore
void unlock_semaphore() {
    sem_post(&g_client_sem);
}

// Function to handle client requests
void *handle_client_request(void *socket_ptr) {
    int client_socket = *((int *)socket_ptr);

    char buffer[MAX_REQUEST_LENGTH];
    while (1) {
        memset(buffer, 0, MAX_REQUEST_LENGTH);
        int bytes_received = recv(client_socket, buffer, MAX_REQUEST_LENGTH, 0);
        if (bytes_received <= 0) {
            break;
        }

        printf("Client %d sent the following request: %s\n", client_socket, buffer);
    }

    close(client_socket);
    return NULL;
}

// Function to accept client connections
void accept_client_connections() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Error creating socket\n");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("Error binding socket\n");
        exit(1);
    }

    if (listen(server_socket, BACKLOG) == -1) {
        printf("Error listening on socket\n");
        exit(1);
    }

    while (1) {
        printf("Waiting for client connection...\n");
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            printf("Error accepting client connection\n");
            continue;
        }

        lock_semaphore();
        g_client_sockets[g_client_count] = client_socket;
        g_client_count++;
        unlock_semaphore();

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client_request, (void *)&client_socket);
    }
}

// Main function
int main() {
    init_semaphore();
    accept_client_connections();

    return 0;
}