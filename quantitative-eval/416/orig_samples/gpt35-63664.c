//FormAI DATASET v1.0 Category: Chat server ; Style: complex
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10

void *connection_handler(void *);

int main(int argc, char const *argv[]) {
    int serverSocket, newSocket, c;
    struct sockaddr_in server, client;
    char *message;

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        printf("Error: Failed to create socket.\n");
        return 1;
    }

    // Prepare sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind
    if (bind(serverSocket, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Error: Failed to bind the socket.\n");
        return 1;
    }

    // Listen
    listen(serverSocket, MAX_CLIENTS);

    // Accept and incoming connection
    printf("Waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);

    while ((newSocket = accept(serverSocket, (struct sockaddr *)&client, (socklen_t *)&c))) {
        printf("Connection accepted.\n");

        // Reply to client
        message = "Connected to chat server.\n";
        write(newSocket, message, strlen(message));

        // Create new thread for the connection
        pthread_t sniffer_thread;
        if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *)&newSocket) < 0) {
            printf("Error: Failed to create thread.\n");
            return 1;
        }

        // Thread detached
        pthread_detach(sniffer_thread);
    }

    if (newSocket < 0) {
        printf("Error: Failed to accept the connection.\n");
        return 1;
    }

    return 0;
}

void *connection_handler(void *socket_desc) {
    // Get socket descriptor
    int newSocket = *(int *)socket_desc;
    char client_message[2000];

    // Send message to client
    char *message = "Welcome to the chat server.\n";
    write(newSocket, message, strlen(message));

    // Receive client message
    while (recv(newSocket, client_message, 2000, 0) > 0) {
        printf("Client: %s", client_message);

        // Send message to client
        printf("Server: ");
        fgets(client_message, 2000, stdin);
        write(newSocket, client_message, strlen(client_message));
    }

    if (recv(newSocket, client_message, 2000, 0) == 0) {
        printf("Client disconnected.\n");
        fflush(stdout);
    } else {
        printf("Error: Failed to receive message from the client.\n");
    }

    // Free the socket pointer
    free(socket_desc);

    return 0;
}