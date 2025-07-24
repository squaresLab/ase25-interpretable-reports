//GEMINI-pro DATASET v1.0 Category: Networking ; Style: cheerful
// A whimsical C networking program to spread joy

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

// Our own special port for sending happiness
#define HAPPY_PORT 9999

// A thread to handle each incoming connection
void* handle_connection(void* arg) {
    int client_socket = *(int*)arg;

    // Receive the message from the client
    char message[1024] = {0};
    recv(client_socket, message, sizeof(message), 0);

    // Spread the joy!
    printf("Received message: %s\n", message);
    send(client_socket, "You're welcome! :)", strlen("You're welcome! :)"), 0);

    // Close the connection
    close(client_socket);
    return NULL;
}

int main() {
    // Create the server socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Failed to create server socket");
        return 1;
    }

    // Bind the socket to our special port
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(HAPPY_PORT);
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Failed to bind server socket");
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Failed to listen on server socket");
        return 1;
    }

    printf("Server is up and running, waiting for connections...\n");

    // Handle incoming connections in separate threads
    int client_socket;
    pthread_t thread;
    while ((client_socket = accept(server_socket, NULL, NULL)) != -1) {
        if (pthread_create(&thread, NULL, handle_connection, (void*)&client_socket) != 0) {
            perror("Failed to create thread");
            return 1;
        }
        pthread_detach(thread);
    }

    // Close the server socket
    close(server_socket);
    return 0;
}