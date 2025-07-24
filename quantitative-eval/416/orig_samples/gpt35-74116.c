//FormAI DATASET v1.0 Category: Wi-Fi Signal Strength Analyzer ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080

// Function to handle each connection
void *handle_connection(void *fd_ptr) {
    int client_fd = *(int *)fd_ptr;
    char buffer[1024];
    bzero(buffer, 1024);
    recv(client_fd, buffer, 1024, 0);
    printf("Received message: %s\n", buffer);

    // Analyze Wi-Fi signal strength here

    send(client_fd, "Response from server: Wi-Fi signal analyzed.", 44, 0);

    close(client_fd);
    pthread_exit(NULL);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Create TCP socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("TCP socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d.\n", PORT);

    // Handle incoming connections concurrently using threads
    pthread_t thread_id;
    while ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen))) {
        if (pthread_create(&thread_id, NULL, handle_connection, &client_fd) != 0) {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
        printf("Connection accepted from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
        pthread_detach(thread_id);
    }

    return 0;
}