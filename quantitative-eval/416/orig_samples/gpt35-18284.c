//FormAI DATASET v1.0 Category: Chat server ; Style: multivariable
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int clients[MAX_CLIENTS];
int num_clients = 0;

pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *client_socket) {
    int sock = *(int *) client_socket;
    char buffer[BUFFER_SIZE];
    while (1) {
        int read_size = read(sock, buffer, BUFFER_SIZE);
        if (read_size < 0) {
            perror("Error reading from client");
            break;
        }
        buffer[read_size] = '\0';
        printf("Client %d says: %s\n", sock, buffer);
        pthread_mutex_lock(&client_mutex);
        for (int i = 0; i < num_clients; i++) {
            if (clients[i] != sock) {
                write(clients[i], buffer, strlen(buffer));
            }
        }
        pthread_mutex_unlock(&client_mutex);
    }
    pthread_mutex_lock(&client_mutex);
    for (int i = 0; i < num_clients; i++) {
        if (clients[i] == sock) {
            clients[i] = clients[num_clients - 1];
            num_clients--;
        }
    }
    pthread_mutex_unlock(&client_mutex);
    printf("Client %d disconnected\n", sock);
    close(sock);
    return NULL;
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Error binding to port");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, MAX_CLIENTS) < 0) {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    printf("Chat server started, waiting for connections on port %d\n", PORT);

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            perror("Error accepting client connection");
            continue;
        }
        pthread_mutex_lock(&client_mutex);
        clients[num_clients] = client_socket;
        num_clients++;
        pthread_mutex_unlock(&client_mutex);
        printf("Client %d connected\n", client_socket);
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, &client_socket) != 0) {
            perror("Error creating thread for client");
            break;
        }
        pthread_detach(thread);
    }

    close(server_socket);
    return 0;
}