//FormAI DATASET v1.0 Category: Chat server ; Style: Sherlock Holmes
// The case of the Chat Server
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8888

void *connection_handler(void *);

int main(void) {
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
        return 1;
    }

    // Prepare socket structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind socket
    if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
        printf("Binding failed");
        return 1;
    }

    // Listen
    listen(socket_desc, 3);
    puts("Server up and running!");

    // Accept incoming connections
    c = sizeof(struct sockaddr_in);
    while ((client_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &c))) {
        puts("New connection accepted");

        pthread_t sniffer_thread;
        if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *) &client_sock) < 0) {
            printf("Could not create thread");
            return 1;
        }

        puts("Handler assigned");
    }

    if (client_sock < 0) {
        printf("Accept failed");
        return 1;
    }

    return 0;
}

void *connection_handler(void *socket_desc) {
    int sock = *(int *) socket_desc;
    int read_size;
    char *message, client_message[2000];

    // Welcome message
    message = "Connected to the Chat Server! Say something...\n";
    write(sock, message, strlen(message));

    // Handle client messages
    while ((read_size = recv(sock, client_message, 2000, 0)) > 0) {
        // Add null terminator to message
        client_message[read_size] = '\0';

        // Echo back message to client
        write(sock, client_message, strlen(client_message));

        // Clear buffer
        memset(client_message, 0, 2000);
    }

    if (read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    } else if (read_size == -1) {
        printf("Receive failed");
    }

    // Free socket pointer
    free(socket_desc);

    return 0;
}