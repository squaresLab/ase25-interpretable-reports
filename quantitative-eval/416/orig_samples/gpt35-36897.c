//FormAI DATASET v1.0 Category: Networking ; Style: complex
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void *handle_client(void *socket_desc_ptr);

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address = {0};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        perror("Failed to bind socket to address");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) == -1) {
        perror("Failed to listen for incoming connections");
        exit(EXIT_FAILURE);
    }

    while (true) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            perror("Failed to accept incoming connection");
            continue;
        }

        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handle_client, &client_socket) != 0) {
            perror("Failed to create new thread for client");
            close(client_socket);
        }
    }
}

void *handle_client(void *socket_desc_ptr) {
    int client_socket = *(int *) socket_desc_ptr;

    char buffer[BUFFER_SIZE];
    ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received == -1) {
        perror("Failed to receive data from client");
        close(client_socket);
        pthread_exit(NULL);
    }

    printf("Received message from client: %s\n", buffer);

    const char *message = "Hello from server!";
    ssize_t bytes_sent = send(client_socket, message, strlen(message) + 1, 0);
    if (bytes_sent == -1) {
        perror("Failed to send data to client");
    }

    close(client_socket);
    pthread_exit(NULL);
}