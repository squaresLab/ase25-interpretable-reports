//Falcon-180B DATASET v1.0 Category: Graph Coloring Problem ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define MAX_MSG_SIZE 1024

int server_socket;
int client_sockets[MAX_CLIENTS];
int num_clients;
int client_color[MAX_CLIENTS];

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[MAX_MSG_SIZE];

    while (1) {
        memset(buffer, 0, MAX_MSG_SIZE);
        int bytes_received = recv(client_socket, buffer, MAX_MSG_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        printf("Client %d sent message: %s\n", client_socket, buffer);

        // Broadcast the message to all other clients
        for (int i = 0; i < num_clients; i++) {
            if (i!= client_socket) {
                send(client_sockets[i], buffer, strlen(buffer), 0);
            }
        }
    }

    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);

    printf("Server started on port 8080\n");

    while (1) {
        socklen_t client_size = sizeof(client_sockets[0]);
        int new_client = accept(server_socket, (struct sockaddr *) &client_sockets[num_clients], &client_size);

        if (new_client > 0) {
            num_clients++;
            client_color[num_clients - 1] = 0;
            printf("Client %d connected\n", new_client);

            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, (void *) &new_client);
        }
    }

    return 0;
}