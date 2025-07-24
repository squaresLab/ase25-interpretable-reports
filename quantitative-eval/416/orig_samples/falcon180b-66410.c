//Falcon-180B DATASET v1.0 Category: Smart home automation ; Style: asynchronous
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int server_socket;
int client_sockets[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *socket_desc) {
    int client_socket = *(int *) socket_desc;
    char buffer[BUFFER_SIZE];

    while(1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if(bytes_received <= 0) {
            break;
        }

        printf("Client %d sent: %s\n", client_socket, buffer);
    }

    close(client_socket);
    return NULL;
}

void accept_connections() {
    while(1) {
        int client_socket = accept(server_socket, NULL, NULL);

        if(client_socket == -1) {
            break;
        }

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, &client_socket);
        client_sockets[num_clients++] = client_socket;
    }
}

int main() {
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    while(1) {
        int client_socket = accept(server_socket, NULL, NULL);

        if(client_socket == -1) {
            break;
        }

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, &client_socket);
        client_sockets[num_clients++] = client_socket;
    }

    return 0;
}