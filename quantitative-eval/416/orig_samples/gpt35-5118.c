//FormAI DATASET v1.0 Category: Chat server ; Style: Romeo and Juliet
/*
* A Chat Server example program 
* Written in the style of Romeo and Juliet
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// Global variables
int server_socket;                  // File descriptor for server socket
int client_socket[5] = {0};         // File descriptor array of connected clients
int num_clients = 0;                // Number of connected clients
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex lock for thread safety

// Function to handle client connections
void *handle_connection(void* arg) {
    int client_fd = *(int*)arg;     // Client file descriptor
    char buffer[1024] = {0};        // Buffer for messages
    
    // Greetings
    write(client_fd, "My bounty is as boundless as the sea\nMy love as deep; the more I give to thee\nThe more I have, for both are infinite.\n", 1024);
    write(client_fd, "Welcome to our chat! Tell me, dear, who art thou?\n", 1024);

    // Loop to receive messages and broadcast to other clients
    while(read(client_fd, buffer, 1024) != 0) {
        // Lock mutex to prevent multiple clients from writing at the same time
        pthread_mutex_lock(&mutex);
        
        // Broadcast message to all other clients
        for(int i=0; i<num_clients; i++) {
            if(client_socket[i] != client_fd) {
                write(client_socket[i], buffer, strlen(buffer));
                write(client_socket[i], "\n", 1);
            }
        }
        
        // Unlock mutex
        pthread_mutex_unlock(&mutex);
    }
    
    // Cleanup and goodbyes
    write(client_fd, "Good night, good night! Parting is such sweet sorrow\nThat I shall say good night till it be morrow.\n", 1024);
    close(client_fd);
    pthread_exit(NULL);
}

int main() {
    struct sockaddr_in server_address;
    int address_len = sizeof(server_address);    // Address length
    
    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Set server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8888);
    
    // Bind server socket
    if(bind(server_socket, (struct sockaddr *)&server_address, address_len) == -1) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    if(listen(server_socket, 5)) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Server running on port 8888...\n");

    // Loop to handle incoming connections
    while(1) {
        // Accept incoming connection
        int client_fd = accept(server_socket, (struct sockaddr *)&server_address, (socklen_t*)&address_len);
        if (client_fd == -1) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }
        
        // Add client to client_socket array
        pthread_mutex_lock(&mutex);
        client_socket[num_clients++] = client_fd;
        pthread_mutex_unlock(&mutex);
        
        // Start thread to handle client messages
        pthread_t thread;
        if(pthread_create(&thread, NULL, handle_connection, (void*)&client_fd)) {
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }
    
    return 0;
}