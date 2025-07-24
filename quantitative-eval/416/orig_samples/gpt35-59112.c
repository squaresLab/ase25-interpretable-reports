//FormAI DATASET v1.0 Category: Client Server Application ; Style: multi-threaded
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

pthread_t thread_ids[MAX_CLIENTS];
int client_socket[MAX_CLIENTS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *);

int main(int argc, char *argv[]) {
    int server_socket, client_count = 0;
    struct sockaddr_in server_address, client_address;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    // Creating socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Setting up server address information
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8888);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // Binding socket to server address
    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) != 0) {
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) != 0) {
        perror("Failed to listen on socket");
        exit(EXIT_FAILURE);
    }

    printf("Server started, waiting for incoming connections...\n");

    // Accepting incoming connections from clients
    while (client_count < MAX_CLIENTS) {
        socklen_t client_address_len = sizeof(client_address);
        int client_socket_fd = accept(server_socket, (struct sockaddr *) &client_address, &client_address_len);

        if (client_socket_fd == -1) {
            perror("Failed to accept client connection");
            continue;
        }

        printf("Client %d connected\n", client_count + 1);

        // Locking mutex to prevent race condition between threads
        pthread_mutex_lock(&mutex);

        // Creating a new thread to handle client requests
        if (pthread_create(&thread_ids[client_count], NULL, handle_client, &client_socket_fd) != 0) {
            perror("Failed to create thread");
            exit(EXIT_FAILURE);
        }

        client_socket[client_count] = client_socket_fd;
        client_count++;

        // Unlocking mutex
        pthread_mutex_unlock(&mutex);
    }

    // Closing all client sockets and joining threads
    for (int i = 0; i < MAX_CLIENTS; i++) {
        pthread_join(thread_ids[i], NULL);
        close(client_socket[i]);
    }

    // Closing server socket
    close(server_socket);

    return 0;
}

void *handle_client(void *sock_fd) {
    int client_socket_fd = *(int *) sock_fd;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    while (true) {
        ssize_t data_size = recv(client_socket_fd, buffer, BUFFER_SIZE, 0);

        if (data_size == -1) {
            perror("Failed to receive data from client");
            break;
        } else if (data_size == 0) {
            printf("Client disconnected\n");
            break;
        }

        printf("Received message from client: %s", buffer);

        if (send(client_socket_fd, buffer, strlen(buffer), 0) == -1) {
            perror("Failed to send message to client");
            break;
        }

        // Resetting buffer
        memset(buffer, 0, BUFFER_SIZE);
    }

    close(client_socket_fd);
    pthread_exit(NULL);
}