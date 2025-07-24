//FormAI DATASET v1.0 Category: Socket programming ; Style: sophisticated
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080

void *handle_client(void *client_socket) {
    int socket = *(int *)client_socket;  
    char buffer[1024] = {0};
    while (recv(socket, buffer, 1024, 0) > 0) {
        printf("Message from client: %s\n", buffer);
        memset(buffer, 0, 1024);
    }
    close(socket);
    return NULL;
}

int main(int argc, char const *argv[])
{
    int server_socket;
    struct sockaddr_in address;

    // Create socket file descriptor
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Set socket options
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    // Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    if (listen(server_socket, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server running on port %d\n", PORT);
    while(1) {
        struct sockaddr_in client_address;
        int addrlen = sizeof(client_address);

        // Accept incoming connection
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, (socklen_t*)&addrlen);
        if (client_socket < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }
        
        printf("Client connected: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket) < 0) {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}