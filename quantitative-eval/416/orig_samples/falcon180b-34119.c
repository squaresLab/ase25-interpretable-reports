//Falcon-180B DATASET v1.0 Category: Chat server ; Style: grateful
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

#define SERVER_PORT 8080
#define MAX_CLIENTS 10
#define MAX_MSG_LEN 1024

int client_sockets[MAX_CLIENTS];
int num_clients = 0;

void* handle_client(void* arg) {
    int client_socket = *((int*) arg);
    char buffer[MAX_MSG_LEN];

    while (1) {
        memset(buffer, 0, MAX_MSG_LEN);
        int bytes_received = recv(client_socket, buffer, MAX_MSG_LEN, 0);

        if (bytes_received <= 0) {
            close(client_socket);
            break;
        }

        printf("Client %d sent message: %s\n", client_socket, buffer);

        // Broadcast the message to all connected clients
        for (int i = 0; i < num_clients; i++) {
            if (client_socket!= client_sockets[i]) {
                send(client_sockets[i], buffer, strlen(buffer), 0);
            }
        }
    }

    return NULL;
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);

    printf("Server started on port %d\n", SERVER_PORT);

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);

        if (num_clients >= MAX_CLIENTS) {
            close(client_socket);
            printf("Server is full. Please try again later.\n");
            continue;
        }

        client_sockets[num_clients] = client_socket;
        num_clients++;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void*) &client_socket);
    }

    return 0;
}