//FormAI DATASET v1.0 Category: Chat server ; Style: thoughtful
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENTS 50
#define BUFFER_SIZE 1024

int clients[MAX_CLIENTS];
int num_clients = 0;

void *client_handler(void *socket_desc) {
    // Get client socket descriptor
    int client_socket = *(int*)socket_desc;
    char client_message[BUFFER_SIZE];
    int read_size;

    // Send message to client to confirm connection
    char *connection_message = "Connected! Type 'exit' to quit.\n";
    send(client_socket, connection_message, strlen(connection_message), 0);

    // Receive message from client
    while ((read_size = recv(client_socket, client_message, BUFFER_SIZE, 0)) > 0) {
        // Check if client wants to exit
        if (strcmp(client_message, "exit\n") == 0) {
            // Remove client from list of active clients
            for (int i = 0; i < num_clients; i++) {
                if (clients[i] == client_socket) {
                    for (int j = i; j < num_clients - 1; j++) {
                        clients[j] = clients[j + 1];
                    }
                    num_clients--;
                    break;
                }
            }
            close(client_socket);
            return NULL;
        }
        // Broadcast message to all clients
        for (int i = 0; i < num_clients; i++) {
            if (clients[i] != client_socket) {
                send(clients[i], client_message, strlen(client_message), 0);
            }
        }
        memset(client_message, 0, BUFFER_SIZE);
    }

    // If client disconnected, remove from list of active clients and exit thread
    for (int i = 0; i < num_clients; i++) {
        if (clients[i] == client_socket) {
            for (int j = i; j < num_clients - 1; j++) {
                clients[j] = clients[j + 1];
            }
            num_clients--;
            break;
        }
    }
    close(client_socket);
    return NULL;
}

int main(int argc, char *argv[]) {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    pthread_t thread_id;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Failed to create socket\n");
        exit(EXIT_FAILURE);
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // Bind socket to server address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Failed to bind socket to server address\n");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    listen(server_socket, 3);

    // Accept incoming connections and create new thread to handle each one
    while ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t*)&client_addr)) > 0) {
        // Add client socket to list of active clients
        clients[num_clients] = client_socket;
        num_clients++;

        // Create new thread to handle client communication
        if (pthread_create(&thread_id, NULL, client_handler, (void*)&client_socket) < 0) {
            printf("Failed to create thread for client\n");
            exit(EXIT_FAILURE);
        }
        printf("New client connected\n");
    }

    // Close server socket
    close(server_socket);

    return 0;
}