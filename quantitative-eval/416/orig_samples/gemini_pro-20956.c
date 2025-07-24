//GEMINI-pro DATASET v1.0 Category: Smart home automation ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#define PORT 9000
#define MAX_CLIENTS 10
#define BUFFER_SIZE 256

int client_sockets[MAX_CLIENTS];
int num_clients = 0;

pthread_mutex_t client_sockets_mutex;

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    char buffer[BUFFER_SIZE];
    int bytes_received;

    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        // Process the received message from the client
        printf("Message received from client: %s\n", buffer);

        // Send a response to the client
        send(client_socket, "Message received", strlen("Message received"), 0);
    }

    // Close the client socket
    pthread_mutex_lock(&client_sockets_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] == client_socket) {
            client_sockets[i] = -1;
            num_clients--;
            break;
        }
    }
    pthread_mutex_unlock(&client_sockets_mutex);

    close(client_socket);

    return NULL;
}

int main() {
    // Initialize the server socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind the server socket to the server address
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Initialize the mutex for protecting the client sockets array
    pthread_mutex_init(&client_sockets_mutex, NULL);

    // Main loop for handling client connections
    while (1) {
        // Accept an incoming connection
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            perror("accept");
            continue;
        }

        // Add the client socket to the array of client sockets
        pthread_mutex_lock(&client_sockets_mutex);
        if (num_clients >= MAX_CLIENTS) {
            // Too many clients connected, reject the connection
            close(client_socket);
            pthread_mutex_unlock(&client_sockets_mutex);
            continue;
        }

        client_sockets[num_clients++] = client_socket;
        pthread_mutex_unlock(&client_sockets_mutex);

        // Create a new thread to handle the client
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handle_client, &client_socket) != 0) {
            perror("pthread_create");
            close(client_socket);
        }
    }

    // Clean up
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] != -1) {
            close(client_sockets[i]);
        }
    }

    pthread_mutex_destroy(&client_sockets_mutex);
    close(server_socket);

    return EXIT_SUCCESS;
}