//Falcon-180B DATASET v1.0 Category: Networking ; Style: complex
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

// Function to handle client connection
void *handle_client(void *socket_desc) {
    int client_socket = *(int *) socket_desc;
    char buffer[BUFFER_SIZE];

    while(1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if(bytes_received <= 0) {
            printf("Client disconnected\n");
            close(client_socket);
            pthread_exit(NULL);
        }

        printf("Received message: %s\n", buffer);

        // Send response to client
        send(client_socket, "Response from server\n", strlen("Response from server\n"), 0);
    }

    return NULL;
}

// Function to create socket and listen for client connections
int create_socket() {
    int socket_fd;
    struct sockaddr_in server_addr;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_fd == -1) {
        printf("Socket creation failed\n");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    if(bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("Bind failed\n");
        exit(1);
    }

    if(listen(socket_fd, 5) == -1) {
        printf("Listen failed\n");
        exit(1);
    }

    printf("Socket created and listening on port %d\n", SERVER_PORT);

    return socket_fd;
}

// Main function to start server and handle client connections
int main() {
    int socket_fd = create_socket();

    while(1) {
        struct sockaddr_in client_addr;
        socklen_t addr_size = sizeof(client_addr);
        int client_socket = accept(socket_fd, (struct sockaddr *)&client_addr, &addr_size);

        if(client_socket == -1) {
            printf("Client connection failed\n");
            continue;
        }

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket);
    }

    return 0;
}