//Falcon-180B DATASET v1.0 Category: Smart home automation ; Style: decentralized
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define PORT 8080

int clients[MAX_CLIENTS];
int num_clients = 0;
int server_socket;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            close(client_socket);
            break;
        }

        printf("Client %d sent message: %s\n", client_socket, buffer);
    }

    return NULL;
}

int main() {
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    while (num_clients < MAX_CLIENTS) {
        socklen_t client_size = sizeof(clients[num_clients]);
        int client_socket = accept(server_socket, (struct sockaddr *) &clients[num_clients], &client_size);

        if (client_socket == -1) {
            printf("Error accepting client\n");
        } else {
            printf("Client %d connected\n", client_socket);
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
            num_clients++;
        }
    }

    return 0;
}