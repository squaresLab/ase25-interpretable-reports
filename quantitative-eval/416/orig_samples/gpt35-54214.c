//FormAI DATASET v1.0 Category: Chat server ; Style: distributed
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define PORT 8080

void *client_handler(void *);

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    pthread_t thread_id;
    
    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }
    
    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Failed to set socket options");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind server socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }
    
    // Listen for connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Failed to listen for connections");
        exit(EXIT_FAILURE);
    }
    
    printf("Server started on port %d...\n", PORT);
    
    // Accept incoming connections
    while ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))) {
        printf("New client connected: %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
        
        // Create a new thread to handle the client connection
        if (pthread_create(&thread_id, NULL, client_handler, (void *)&new_socket) < 0) {
            perror("Failed to create thread to handle client");
            exit(EXIT_FAILURE);
        }
        
        // Detach the thread so it can continue running independently
        pthread_detach(thread_id);
    }
    
    return 0;
}

void *client_handler(void *socket_desc) {
    // Cast socket descriptor to int
    int socket = *(int*)socket_desc;
    char buffer[1024] = {0};
    int bytes_read;
    
    // Loop to receive and process messages from client
    while ((bytes_read = read(socket, buffer, sizeof(buffer))) > 0) {
        printf("Received message from client[%d]: %s\n", socket, buffer);
        // Echo message back to client
        if (write(socket, buffer, strlen(buffer)) < 0) {
            perror("Failed to send message to client");
            exit(EXIT_FAILURE);
        }
        memset(buffer, 0, 1024);
    }
    
    printf("Client[%d] disconnected.\n", socket);
    close(socket);
    pthread_exit(NULL);
}