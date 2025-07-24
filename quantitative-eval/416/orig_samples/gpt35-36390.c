//FormAI DATASET v1.0 Category: Socket programming ; Style: complex
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 50

int client_sockets[MAX_CLIENTS];
pthread_t threads[MAX_CLIENTS];

void *handle_client(void *arg) {
    int client_socket = *((int *) arg);
    char buffer[1024];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_read = read(client_socket, buffer, sizeof(buffer));
        if (bytes_read < 0) {
            perror("Error reading from client");
            close(client_socket);
            return NULL;
        }
        if (bytes_read == 0 || strcmp(buffer, "quit\n") == 0) {
            printf("Client disconnected\n");
            close(client_socket);
            return NULL;
        }
        printf("Received message: %s", buffer);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] > 0 && client_sockets[i] != client_socket) {
                write(client_sockets[i], buffer, strlen(buffer));
            }
        }
    }
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error creating socket");
        return 1;
    }
    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("Error binding socket");
        return 1;
    }
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Error listening on socket");
        return 1;
    }
    printf("Server started on port %d\n", PORT);
    int index = 0;
    while (1) {
        int client_size = sizeof(client_address);
        if ((client_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_size)) < 0) {
            perror("Error accepting client connection");
            return 1;
        }
        printf("Accepted new connection from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        if (index == MAX_CLIENTS) {
            printf("Max clients reached\n");
            close(client_socket);
        } else {
            client_sockets[index] = client_socket;
            pthread_create(&threads[index], NULL, handle_client, (void *) &client_socket);
            index++;
        }
    }
    return 0;
}