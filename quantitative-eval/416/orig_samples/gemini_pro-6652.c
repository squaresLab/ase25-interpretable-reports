//GEMINI-pro DATASET v1.0 Category: TCP/IP Programming ; Style: scalable
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CONNECTIONS 10

// Function to handle client connections
void *handle_client(void *arg) {
    int client_socket = *((int *)arg);

    // Receive data from client
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received < 0) {
        perror("recv");
        close(client_socket);
        return NULL;
    }

    // Process received data
    // ...

    // Send data back to client
    char *response = "Hello from server!";
    int bytes_sent = send(client_socket, response, strlen(response), 0);
    if (bytes_sent < 0) {
        perror("send");
        close(client_socket);
        return NULL;
    }

    // Close client socket
    close(client_socket);

    return NULL;
}

int main() {
    // Create a server socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    // Set socket options
    int optval = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // Bind socket to address and port
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5000);
    server_address.sin_addr.s_addr = INADDR_ANY;
    int bind_result = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (bind_result < 0) {
        perror("bind");
        close(server_socket);
        return EXIT_FAILURE;
    }

    // Listen for connections
    int listen_result = listen(server_socket, MAX_CONNECTIONS);
    if (listen_result < 0) {
        perror("listen");
        close(server_socket);
        return EXIT_FAILURE;
    }

    // Accept connections in a loop
    while (1) {
        // Create a client socket
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
        if (client_socket < 0) {
            perror("accept");
            continue;
        }

        // Create a new thread to handle the client connection
        pthread_t client_thread;
        int create_thread_result = pthread_create(&client_thread, NULL, handle_client, (void *)&client_socket);
        if (create_thread_result != 0) {
            perror("pthread_create");
            close(client_socket);
            continue;
        }

        // Detach the client thread
        if (pthread_detach(client_thread) != 0) {
            perror("pthread_detach");
        }
    }

    // Close server socket
    close(server_socket);

    return EXIT_SUCCESS;
}