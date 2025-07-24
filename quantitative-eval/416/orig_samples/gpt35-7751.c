//FormAI DATASET v1.0 Category: Networking ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// Global variables
int server_socket;
int client_sockets[MAX_CLIENTS];
int num_clients = 0;

// Thread function to handle each client connection
void *handle_client(void *arg) {
    int client_socket = *(int*)arg;
    char buffer[BUFFER_SIZE];

    // Receive messages until client disconnects
    while (1) {
        // Receive message from client
        int data_len = recv(client_socket, buffer, BUFFER_SIZE, 0);

        // Check for disconnect
        if (data_len == 0) {
            printf("Client disconnected\n");
            close(client_socket);

            // Remove client from array of active clients
            for (int i = 0; i < num_clients; i++) {
                if (client_sockets[i] == client_socket) {
                    client_sockets[i] = client_sockets[num_clients-1];
                    num_clients--;
                    break;
                }
            }

            // Exit thread
            pthread_exit(NULL);
        }

        // Print message
        printf("Received: %s", buffer);

        // Forward message to all other clients
        for (int i = 0; i < num_clients; i++) {
            int socket_fd = client_sockets[i];

            // Skip if this is the original client
            if (socket_fd == client_socket) {
                continue;
            }

            // Send message to other clients
            if (send(socket_fd, buffer, data_len, 0) != data_len) {
                printf("Error sending message to client\n");
            }
        }
    }
}

int main() {
    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Error creating server socket\n");
        return 1;
    }

    // Set socket options to reuse address
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        printf("Error setting socket options\n");
        return 1;
    }

    // Set server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8888);

    // Bind server socket to address
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        printf("Error binding server socket to address\n");
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        printf("Error listening for incoming connections\n");
        return 1;
    }

    printf("Server started\n");

    // Accept incoming connections and spawn thread to handle each one
    while (1) {
        // Accept connection
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
        if (client_socket == -1) {
            printf("Error accepting client connection\n");
            continue;
        }

        // Add client to array of active clients
        if (num_clients == MAX_CLIENTS) {
            printf("Maximum number of clients reached\n");
            close(client_socket);
            continue;
        }
        client_sockets[num_clients] = client_socket;
        num_clients++;

        // Spawn thread to handle client connection
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, &client_socket) == -1) {
            printf("Error creating thread to handle client connection\n");
            return 1;
        }
    }

    // Close server socket
    close(server_socket);

    return 0;
}