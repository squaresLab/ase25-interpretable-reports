//FormAI DATASET v1.0 Category: Client Server Application ; Style: unmistakable
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8888

// Function to handle communication with the client
void *connection_handler(void *);

int main(int argc, char *argv[]) {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    int opt = 1;
    int addrlen = sizeof(server_addr);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Setting socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Setting server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Binding socket to the specified address and port
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening for incoming connections
    if (listen(server_fd, 1) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    puts("Waiting for incoming connections...");

    // Accepting incoming connections and handling them using threads
    while ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen))) {
        puts("Connection accepted");

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, connection_handler, (void *)&client_fd) < 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    if (client_fd < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    return 0;
}

void *connection_handler(void *socket_fd) {
    int client_fd = *(int *)socket_fd;
    char buffer[1024] = {0};
    char message[1024] = {0};

    // Sending welcome message to client
    strcpy(message, "Welcome to the server!");
    if (send(client_fd, message, strlen(message), 0) < 0) {
        perror("send");
        exit(EXIT_FAILURE);
    }

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        memset(message, 0, sizeof(message));

        // Receiving message from client
        if (recv(client_fd, buffer, sizeof(buffer), 0) < 0) {
            perror("recv");
            exit(EXIT_FAILURE);
        }

        // Preparing message to be sent back to client
        strcpy(message, "Server received: ");
        strcat(message, buffer);

        // Sending message back to client
        if (send(client_fd, message, strlen(message), 0) < 0) {
            perror("send");
            exit(EXIT_FAILURE);
        }
    }

    // Closing client connection
    close(client_fd);

    return NULL;
}