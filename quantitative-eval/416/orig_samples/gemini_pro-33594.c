//GEMINI-pro DATASET v1.0 Category: TCP/IP Programming ; Style: peaceful
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

// Shared data
int server_socket_fd;
pthread_mutex_t lock;

// Thread function
void *client_handler(void *arg) {
    int client_socket_fd = *(int *)arg;
    char buffer[1024];
    while (1) {
        // Receive data from client
        int num_bytes_received = recv(client_socket_fd, buffer, 1024, 0);
        if (num_bytes_received == 0) {
            printf("Client disconnected\n");
            pthread_mutex_lock(&lock);
            close(client_socket_fd);
            pthread_mutex_unlock(&lock);
            break;
        }
        else if (num_bytes_received < 0) {
            perror("recv");
            pthread_mutex_lock(&lock);
            close(client_socket_fd);
            pthread_mutex_unlock(&lock);
            break;
        }

        // Process data
        pthread_mutex_lock(&lock);
        printf("Received message from client: %s\n", buffer);
        pthread_mutex_unlock(&lock);

        // Send data back to client
        int num_bytes_sent = send(client_socket_fd, "Hello from server!", 17, 0);
        if (num_bytes_sent < 0) {
            perror("send");
            pthread_mutex_lock(&lock);
            close(client_socket_fd);
            pthread_mutex_unlock(&lock);
            break;
        }
    }
    pthread_exit(NULL);
}

int main() {
    // Initialize shared data
    pthread_mutex_init(&lock, NULL);

    // Create server socket
    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_fd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    // Set server socket options
    int opt = 1;
    int result = setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    if (result == -1) {
        perror("setsockopt");
        return EXIT_FAILURE;
    }

    // Bind server socket to port
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080);
    result = bind(server_socket_fd, (struct sockaddr *)&server_address, sizeof(server_address));
    if (result == -1) {
        perror("bind");
        return EXIT_FAILURE;
    }

    // Listen for client connections
    result = listen(server_socket_fd, 5);
    if (result == -1) {
        perror("listen");
        return EXIT_FAILURE;
    }

    printf("Server listening on port 8080\n");

    // Accept client connections in a loop
    while (1) {
        int client_socket_fd;
        struct sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);
        client_socket_fd = accept(server_socket_fd, (struct sockaddr *)&client_address, &client_address_size);
        if (client_socket_fd == -1) {
            perror("accept");
            continue;
        }

        // Create a new thread to handle the client connection
        pthread_t thread;
        pthread_create(&thread, NULL, client_handler, &client_socket_fd);
    }

    // Clean up
    pthread_mutex_destroy(&lock);
    close(server_socket_fd);
    return EXIT_SUCCESS;
}