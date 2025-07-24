//FormAI DATASET v1.0 Category: Chat server ; Style: Donald Knuth
// The following code implements a chat server program in C, inspired by Donald Knuth's literate programming style

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// Define a struct to hold information about connected clients
typedef struct {
    int socket_fd;  // Socket file descriptor of client
    char username[32];  // Username of client
} client_info_t;

// Global variables
int server_socket_fd;
client_info_t clients[MAX_CLIENTS];
int num_clients = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function prototypes
void *handle_client(void *arg);
void broadcast_message(char *message, int exclude_socket_fd);

int main(int argc, char *argv[]) {
    // Create server socket
    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Bind socket to address and port
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(5050);
    bind(server_socket_fd, (struct sockaddr*)&server_address, sizeof(server_address));

    // Listen for incoming connections
    listen(server_socket_fd, MAX_CLIENTS);

    // Accept connections and spawn threads to handle each client
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    while (1) {
        int client_socket_fd = accept(server_socket_fd, (struct sockaddr*)&client_address, &client_address_len);
        if (client_socket_fd < 0) {
            perror("Error accepting client connection");
            continue;
        }

        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, (void*)&client_socket_fd);
    }

    close(server_socket_fd);

    return 0;
}

void *handle_client(void *arg) {
    // Get socket file descriptor from argument
    int socket_fd = *(int*)arg;

    // Receive initial username message from client
    char username[32];
    recv(socket_fd, username, 32, 0);

    // Add client to list of connected clients
    pthread_mutex_lock(&clients_mutex);
    clients[num_clients].socket_fd = socket_fd;
    strcpy(clients[num_clients].username, username);
    num_clients++;
    pthread_mutex_unlock(&clients_mutex);

    // Send welcome message to client
    char welcome_message[BUFFER_SIZE];
    sprintf(welcome_message, "Welcome %s!", username);
    send(socket_fd, welcome_message, strlen(welcome_message), 0);

    // Continuously receive and broadcast messages from client
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int recv_size = recv(socket_fd, buffer, BUFFER_SIZE, 0);

        if (recv_size == 0) {
            // Client disconnected
            close(socket_fd);

            // Remove client from list of connected clients
            pthread_mutex_lock(&clients_mutex);
            int i;
            for (i = 0; i < num_clients; i++) {
                if (clients[i].socket_fd == socket_fd) {
                    break;
                }
            }
            num_clients--;
            for (; i < num_clients; i++) {
                clients[i] = clients[i+1];
            }
            pthread_mutex_unlock(&clients_mutex);

            // Broadcast message about client disconnecting
            char disconnect_message[BUFFER_SIZE];
            sprintf(disconnect_message, "%s has left the chat!", username);
            broadcast_message(disconnect_message, socket_fd);
            break;
        }

        // Broadcast message to all other clients
        char message[BUFFER_SIZE + 32];
        sprintf(message, "%s: %s", username, buffer);
        broadcast_message(message, socket_fd);
    }

    pthread_exit(NULL);
}

void broadcast_message(char *message, int exclude_socket_fd) {
    int i;
    pthread_mutex_lock(&clients_mutex);
    for (i = 0; i < num_clients; i++) {
        int client_socket_fd = clients[i].socket_fd;
        if (client_socket_fd != exclude_socket_fd) {
            send(client_socket_fd, message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}