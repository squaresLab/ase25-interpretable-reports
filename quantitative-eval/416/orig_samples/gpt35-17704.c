//FormAI DATASET v1.0 Category: Networking ; Style: imaginative
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CONNECTIONS 5
#define BUFFER_SIZE 1024

void *handle_client(void *);

int main(int argc, char *argv[])
{
    // Create socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Bind socket to a port
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_CONNECTIONS) < 0)
    {
        perror("Failed to listen");
        exit(EXIT_FAILURE);
    }

    printf("Server running on port %d\n", ntohs(server_address.sin_port));
    printf("Waiting for incoming connections...\n");

    // Accept incoming connections
    int client_socket;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    while ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len)))
    {
        printf("Connection accepted, creating thread for client...\n");

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket) != 0)
        {
            perror("Failed to create thread");
            exit(EXIT_FAILURE);
        }
    }

    // Close server socket
    close(server_socket);

    return 0;
}

void *handle_client(void *_client_socket)
{
    int client_socket = *(int *)_client_socket;

    // Receive message from client
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    ssize_t recv_size = recv(client_socket, buffer, sizeof(buffer), 0);

    if (recv_size < 0)
    {
        perror("Error receiving message from client");
        exit(EXIT_FAILURE);
    }

    printf("Received message from client: '%s'\n", buffer);

    // Send message back to client
    char *message = "Hello from server!";
    ssize_t send_size = send(client_socket, message, strlen(message), 0);

    if (send_size < 0)
    {
        perror("Error sending message to client");
        exit(EXIT_FAILURE);
    }

    printf("Sent message to client: '%s'\n", message);

    // Close client socket
    close(client_socket);

    pthread_exit(NULL);
}