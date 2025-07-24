//FormAI DATASET v1.0 Category: Networking ; Style: scalable
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 9000
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// Structure for client Information
struct client_info {
    int socket_fd;
    struct sockaddr_in address;
    char nickname[20];
};

// Global variables
int client_count = 0;
struct client_info clients[MAX_CLIENTS];

// Function to handle client
void *handleClient(void *arg) {
    // Get the socket file descriptor
    int socket_fd = *(int*)arg;

    // Receive the nickname
    char nickname[20];
    if (recv(socket_fd, nickname, sizeof(nickname), 0) <= 0) {
        perror("Nickname failed to receive");
        close(socket_fd);
        return NULL;
    };
    // store the client information
    struct client_info client;
    client.socket_fd = socket_fd;
    client.address = clients[client_count].address;
    strcpy(client.nickname, nickname);
    clients[client_count++] = client;

    // Welcome message
    char welcome_msg[BUFFER_SIZE];
    sprintf(welcome_msg, "%s has joined the chat\n", nickname);
    for (int i = 0; i < client_count; i++) {
        send(clients[i].socket_fd, welcome_msg, strlen(welcome_msg), 0);
    }

    // Start the chat
    char buffer[BUFFER_SIZE];
    while (1) {
        // Receive message
        int receive_status = recv(socket_fd, buffer, sizeof(buffer), 0);
        if (receive_status <= 0) {
            printf("%s left the chat\n", nickname);
            break;
        }

        // Check if message is exit command
        if (strcmp(buffer, "exit\n") == 0) {
            printf("%s left the chat\n", nickname);
            break;
        }

        // Send message to all clients
        for (int i = 0; i < client_count; i++) {
            if (clients[i].socket_fd != socket_fd) {
                char message[BUFFER_SIZE];
                sprintf(message, "%s: %s", nickname, buffer);
                send(clients[i].socket_fd, message, strlen(message), 0);
            }
        }
    }

    // Remove the client from active client list
    for (int i = 0; i < client_count; i++) {
        if (clients[i].socket_fd == socket_fd) {
            int j;
            for (j = i; j < client_count - 1; j++) {
                clients[j] = clients[j+1];
            }
            client_count--;
            break;
        }
    }

    // Notify all clients
    char leave_msg[BUFFER_SIZE];
    sprintf(leave_msg, "%s left the chat\n", nickname);
    for (int i = 0; i < client_count; i++) {
        send(clients[i].socket_fd, leave_msg, strlen(leave_msg), 0);
    }

    // Close the socket
    close(socket_fd);

    return NULL;
}

// Main function
int main() {
    // Create a socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Bind failed");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_CLIENTS) == -1) {
        perror("Listen failed");
        exit(1);
    }

    printf("Server started on port %d\n", PORT);

    // Loop for accepting clients
    while (1) {
        // Accept a client connection
        struct sockaddr_in client_address;
        socklen_t address_len = sizeof(client_address);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_address, &address_len);
        if (client_fd == -1) {
            perror("Accept failed");
            continue;
        }

        // Check if maximum clients limit has been reached
        if (client_count == MAX_CLIENTS) {
            printf("Maximum clients limit reached. Connection request rejected.\n");
            close(client_fd);
            continue;
        }

        // Create a thread to handle client
        pthread_t thread;
        if (pthread_create(&thread, NULL, handleClient, &client_fd) != 0) {
            perror("Thread creation failed");
            close(client_fd);
        }
        // Detach the thread
        pthread_detach(thread);
    }

    // Close the server socket
    close(server_fd);

    return 0;
}