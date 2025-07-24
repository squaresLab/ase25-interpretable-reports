//GEMINI-pro DATASET v1.0 Category: TCP/IP Programming ; Style: future-proof
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// Maximum number of client connections
#define MAX_CONNECTIONS 5

// Port number to listen on
#define PORT 8080

// Function to handle client connections
void* handle_client(void* arg) {
    int client_sock = *(int*) arg;

    // Receive the client's message
    char buffer[1024];
    int bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
    if (bytes_received <= 0) {
        perror("Error receiving message from client");
        close(client_sock);
        return NULL;
    }

    // Process the client's message
    printf("Received message from client: %s\n", buffer);

    // Send a response to the client
    const char* response = "Hello from the server!";
    int bytes_sent = send(client_sock, response, strlen(response), 0);
    if (bytes_sent <= 0) {
        perror("Error sending message to client");
    }

    // Close the client connection
    close(client_sock);

    return NULL;
}

int main() {
    // Create a socket for listening for client connections
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Error creating server socket");
        return EXIT_FAILURE;
    }

    // Bind the socket to the specified port
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding server socket to port");
        close(server_sock);
        return EXIT_FAILURE;
    }

    // Start listening for client connections
    if (listen(server_sock, MAX_CONNECTIONS) == -1) {
        perror("Error listening for client connections");
        close(server_sock);
        return EXIT_FAILURE;
    }

    printf("Server listening on port %d\n", PORT);

    // Accept client connections and handle them in separate threads
    while (1) {
        int client_sock = accept(server_sock, NULL, NULL);
        if (client_sock == -1) {
            perror("Error accepting client connection");
            continue;
        }

        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, &client_sock) != 0) {
            perror("Error creating thread to handle client connection");
            close(client_sock);
            continue;
        }
    }

    // Close the server socket
    close(server_sock);

    return EXIT_SUCCESS;
}