//FormAI DATASET v1.0 Category: Chat server ; Style: sophisticated
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

// Function to handle client connections
void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    char buffer[1024];

    // Read incoming messages from client
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int read_result = read(client_socket, buffer, sizeof(buffer));
        
        // If client disconnected, close socket and exit thread
        if (read_result == 0) {
            close(client_socket);
            pthread_exit(NULL);
        }
        
        // Display the message from client
        printf("Client: %s", buffer);
        
        // Send message back to client
        write(client_socket, buffer, strlen(buffer));
    }
}

int main() {
    // Create socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("Error creating socket\n");
        exit(1);
    }
    
    // Bind socket to port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080); // Use port 8080
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error binding socket\n");
        exit(1);
    }
    
    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        printf("Error listening for incoming connections\n");
        exit(1);
    }
    
    printf("Server started\n");
    
    // Handle incoming connections from clients
    while (1) {
        struct sockaddr_in client_addr;
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, sizeof(client_addr));
        if (client_socket < 0) {
            printf("Error accepting client connection\n");
            continue; // Try again with another connection
        }
        
        printf("Client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        
        // Create thread to handle client
        pthread_t thread_id;
        int thread_result = pthread_create(&thread_id, NULL, &handle_client, &client_socket);
        if (thread_result < 0) {
            printf("Error creating thread to handle client\n");
            close(client_socket);
            continue; // Try again with another connection
        }
    }
    
    // Close server socket
    close(server_socket);
    return 0;
}