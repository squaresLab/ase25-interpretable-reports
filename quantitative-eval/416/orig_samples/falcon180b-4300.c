//Falcon-180B DATASET v1.0 Category: Remote control Vehicle Simulation ; Style: multiplayer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUF_SIZE 1024
#define MAX_CLIENTS 10
#define DELAY 50

int clients[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[BUF_SIZE];

    while (1) {
        memset(buffer, 0, BUF_SIZE);
        int bytes_received = recv(client_socket, buffer, BUF_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        printf("Client %d sent message: %s\n", client_socket, buffer);
        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
    return NULL;
}

void broadcast(int sender_socket, char *message) {
    for (int i = 0; i < num_clients; i++) {
        if (clients[i]!= sender_socket) {
            send(clients[i], message, strlen(message), 0);
        }
    }
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));

    listen(server_socket, MAX_CLIENTS);

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        socklen_t client_size = sizeof(clients[0]);
        int client_socket = accept(server_socket, (struct sockaddr *) &clients[num_clients], &client_size);

        if (num_clients >= MAX_CLIENTS) {
            close(client_socket);
            printf("Server is full. Cannot accept more clients.\n");
            continue;
        }

        num_clients++;
        printf("Client %d connected\n", client_socket);

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
    }

    close(server_socket);
    return 0;
}