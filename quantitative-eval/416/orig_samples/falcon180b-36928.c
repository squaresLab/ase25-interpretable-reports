//Falcon-180B DATASET v1.0 Category: Client Server Application ; Style: real-life
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define MAX_REQUEST_SIZE 1024

int server_socket;
int client_sockets[MAX_CLIENTS];
int num_clients = 0;

void *handle_request(void *arg) {
    int client_socket = *(int *) arg;
    char request[MAX_REQUEST_SIZE];

    while (1) {
        memset(request, 0, MAX_REQUEST_SIZE);
        int bytes_received = recv(client_socket, request, MAX_REQUEST_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        printf("Client %d requested: %s\n", client_socket, request);

        // do something with the request

        send(client_socket, "Request processed.", strlen("Request processed."), 0);
    }

    close(client_socket);
    num_clients--;
}

void accept_connections() {
    while (num_clients < MAX_CLIENTS) {
        int client_socket = accept(server_socket, NULL, NULL);

        if (client_socket == -1) {
            printf("Error accepting connection.\n");
            continue;
        }

        client_sockets[num_clients] = client_socket;
        num_clients++;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_request, (void *) &client_socket);
    }
}

int main() {
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    while (1) {
        accept_connections();
    }

    return 0;
}