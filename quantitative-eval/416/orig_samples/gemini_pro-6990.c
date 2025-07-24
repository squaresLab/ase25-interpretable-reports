//GEMINI-pro DATASET v1.0 Category: Networking ; Style: detailed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CONNECTIONS 5
#define BUFFER_SIZE 1024

// Thread function to handle client requests
void *handle_client(void *arg) {
    int client_socket = *(int *)arg;

    // Receive a message from the client
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

    // Check if the client has closed the connection
    if (bytes_received == 0) {
        printf("Client has closed the connection\n");
        close(client_socket);
        pthread_exit(NULL);
    }

    // Process the message
    printf("Received message from client: %s\n", buffer);
    char response[BUFFER_SIZE];
    sprintf(response, "Hello, client! You sent the message: %s", buffer);

    // Send a response to the client
    int bytes_sent = send(client_socket, response, strlen(response), 0);

    // Close the client socket
    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    // Create a server socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    // Set the server socket to be reusable
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Bind the server socket to the port
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("bind");
        close(server_socket);
        return EXIT_FAILURE;
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_CONNECTIONS) == -1) {
        perror("listen");
        close(server_socket);
        return EXIT_FAILURE;
    }

    // Main loop to handle client connections
    while (1) {
        // Accept an incoming connection
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            perror("accept");
            continue;
        }

        // Create a new thread to handle the client request
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, (void *)&client_socket) != 0) {
            perror("pthread_create");
            close(client_socket);
            continue;
        }

        // Detach the thread so that it can run independently of the main thread
        pthread_detach(thread);
    }

    // Close the server socket
    close(server_socket);

    return EXIT_SUCCESS;
}