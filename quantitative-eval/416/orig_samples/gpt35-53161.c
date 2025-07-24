//FormAI DATASET v1.0 Category: Client Server Application ; Style: puzzling
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

void *handle_client(void *socket_desc);

int main() {
    int socket_desc, new_socket, c;
    struct sockaddr_in server, client;

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        perror("Could not create socket");
        return 1;
    }
    puts("Socket created!");

    // Prepare server sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind
    if (bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("Bind failed");
        return 1;
    }
    puts("Bind done!");

    // Listen
    listen(socket_desc, 3);

    // Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while (new_socket = accept(socket_desc, (struct sockaddr*)&client, (socklen_t*)&c)) {
        pthread_t thread_id;
        printf("Connection accepted from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        if (pthread_create(&thread_id, NULL, handle_client, (void*)&new_socket) < 0) {
            perror("Could not create thread");
            return 1;
        }
        printf("Handler assigned to thread %ld\n", thread_id);
    }

    if (new_socket < 0) {
        perror("Accept failed");
        return 1;
    }
    return 0;
}

void *handle_client(void *socket_desc) {
    int sock = *(int*)socket_desc;
    int read_size;
    char message[1024], client_message[1024];

    // Send welcome message to the client
    strcpy(message, "Welcome to the puzzling server!\n");
    send(sock, message, strlen(message), 0);

    // Receive client's message repeatedly until the client disconnects
    while ((read_size = recv(sock, client_message, 1024, 0)) > 0) {
        // Handle the client's message
        if (strncmp(client_message, "What is the answer to life, the universe, and everything?", 53) == 0) {
            strcpy(message, "The answer to life, the universe, and everything is 42\n");
        } else {
            strcpy(message, "I'm sorry, I cannot answer that question\n");
        }
        // Send the response to the client
        send(sock, message, strlen(message), 0);
        // Clear the buffer
        memset(client_message, 0, sizeof(client_message));
    }

    if (read_size == 0) {
        puts("Client disconnected");
    } else if (read_size == -1) {
        perror("Receive failed");
        return (void*)1;
    }

    // Free the socket descriptor
    free(socket_desc);

    return 0;
}