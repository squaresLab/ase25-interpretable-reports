//Falcon-180B DATASET v1.0 Category: Chat server ; Style: light-weight
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

// Struct to hold client information
typedef struct {
    int sock;
    char name[10];
} client_t;

// Array of clients
client_t clients[MAX_CLIENTS];
int num_clients = 0;

// Function to broadcast message to all clients
void broadcast_message(char *message, int sender_sock) {
    for (int i = 0; i < num_clients; i++) {
        if (clients[i].sock!= sender_sock) {
            send(clients[i].sock, message, strlen(message), 0);
        }
    }
}

// Function to handle client connections
void *handle_client(void *arg) {
    int client_sock = *(int *) arg;
    char buffer[BUFFER_SIZE];

    // Get client name
    char name_buffer[10];
    recv(client_sock, name_buffer, sizeof(name_buffer), 0);
    strcpy(clients[num_clients].name, name_buffer);

    // Send welcome message to client
    sprintf(buffer, "Welcome to the chat, %s!\n", clients[num_clients].name);
    send(client_sock, buffer, strlen(buffer), 0);

    // Notify other clients of new arrival
    sprintf(buffer, "%s has joined the chat.\n", clients[num_clients].name);
    broadcast_message(buffer, client_sock);

    while (1) {
        // Receive message from client
        recv(client_sock, buffer, BUFFER_SIZE, 0);

        // Broadcast message to all clients
        broadcast_message(buffer, client_sock);
    }
}

// Main function
int main(int argc, char **argv) {
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);

    // Set server socket options
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(server_sock, MAX_CLIENTS);

    printf("Chat server started on port 8080\n");

    while (1) {
        // Accept client connection
        int client_sock = accept(server_sock, NULL, NULL);

        // Spawn thread to handle client
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, &client_sock);
    }

    close(server_sock);
    return 0;
}