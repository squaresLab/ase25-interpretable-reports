//FormAI DATASET v1.0 Category: Client Server Application ; Style: post-apocalyptic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10

// Global variables
int client_count = 0;
int client_socket[MAX_CLIENTS];
pthread_mutex_t client_mutex;
pthread_t thread_id[MAX_CLIENTS];

// Function for adding a new client to the client list
void add_client(int socket) {
    pthread_mutex_lock(&client_mutex);
    client_socket[client_count++] = socket;
    pthread_mutex_unlock(&client_mutex);
}

// Function for removing a client from the client list
void remove_client(int index) {
    pthread_mutex_lock(&client_mutex);
    for (int i = index; i < client_count - 1; i++) {
        client_socket[i] = client_socket[i+1];
    }
    client_count--;
    pthread_mutex_unlock(&client_mutex);
}

// Function for handling client connections
void *handle_client(void *arg) {
    int index = *(int *)arg;
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    read(client_socket[index], buffer, sizeof(buffer));
    printf("Client %d connected: %s\n", index, buffer);
    while (1) {
        // Read client message
        memset(buffer, 0, sizeof(buffer));
        int read_size = read(client_socket[index], buffer, sizeof(buffer));
        if (read_size == 0) { // Client disconnected
            printf("Client %d disconnected\n", index);
            remove_client(index);
            break;
        }
        // Print client message
        printf("Client %d says: %s\n", index, buffer);
        // Send message to other clients
        pthread_mutex_lock(&client_mutex);
        for (int i = 0; i < client_count; i++) {
            if (i != index) {
                write(client_socket[i], buffer, sizeof(buffer));
            }
        }
        pthread_mutex_unlock(&client_mutex);
    }
    close(client_socket[index]);
    return NULL;
}

// Main function
int main(int argc, char **argv) {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    pthread_t thread;

    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set server address and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8888);

    // Bind server socket to server address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for connections...\n");

    // Accept incoming connections
    while (1) {
        client_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == -1) {
            perror("accept");
            continue;
        }
        printf("Accepted new connection...\n");
        if (client_count == MAX_CLIENTS) {
            printf("Too many clients\n");
            close(client_socket);
            continue;
        }
        // Add client to client list
        add_client(client_socket);
        int index = client_count - 1;
        // Create thread for handling client
        if (pthread_create(&thread_id[index], NULL, handle_client, (void *)&index) == -1) {
            perror("pthread_create");
            remove_client(index);
            close(client_socket);
            continue;
        }
    }

    return 0;
}