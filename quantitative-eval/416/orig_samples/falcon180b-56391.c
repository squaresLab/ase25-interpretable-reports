//Falcon-180B DATASET v1.0 Category: Smart home light control ; Style: realistic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define PORT 8080

int listening_socket;
int client_sockets[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[1024];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            close(client_socket);
            break;
        }
        printf("Client %d sent: %s\n", client_socket, buffer);
        send(client_socket, buffer, strlen(buffer), 0);
    }
    close(client_socket);
}

void accept_connections() {
    while (1) {
        int client_socket = accept(listening_socket, NULL, NULL);
        if (client_socket == -1) {
            printf("Error accepting connection\n");
        } else {
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
            client_sockets[num_clients++] = client_socket;
        }
    }
}

int main() {
    int opt = 1;
    setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    while (num_clients < MAX_CLIENTS) {
        int client_socket = accept(listening_socket, NULL, NULL);
        if (client_socket == -1) {
            printf("Error accepting connection\n");
        } else {
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
            client_sockets[num_clients++] = client_socket;
        }
    }

    accept_connections();

    return 0;
}