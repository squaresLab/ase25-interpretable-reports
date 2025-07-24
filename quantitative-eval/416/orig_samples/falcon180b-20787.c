//Falcon-180B DATASET v1.0 Category: Chat server ; Style: decentralized
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define MAX_MESSAGE_SIZE 1024

int server_socket;
int client_sockets[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char message[MAX_MESSAGE_SIZE];

    while (1) {
        memset(message, 0, MAX_MESSAGE_SIZE);
        int bytes_received = recv(client_socket, message, MAX_MESSAGE_SIZE, 0);

        if (bytes_received <= 0) {
            printf("Client %d disconnected\n", client_socket);
            close(client_socket);
            pthread_exit(0);
        }

        printf("Client %d sent message: %s\n", client_socket, message);
    }
}

void accept_connections() {
    while (num_clients < MAX_CLIENTS) {
        socklen_t client_size = sizeof(client_sockets[num_clients]);
        int client_socket = accept(server_socket, (struct sockaddr *) &client_sockets[num_clients], &client_size);

        if (client_socket == -1) {
            printf("Error accepting connection\n");
            continue;
        }

        printf("Client %d connected\n", client_socket);
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
        num_clients++;
    }
}

int main() {
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    while (1) {
        accept_connections();
        usleep(100000);
    }

    return 0;
}