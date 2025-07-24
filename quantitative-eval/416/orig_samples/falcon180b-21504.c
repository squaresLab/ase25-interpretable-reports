//Falcon-180B DATASET v1.0 Category: Metadata Extractor ; Style: multiplayer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define MAX_REQUEST_SIZE 1024

int client_sockets[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char request[MAX_REQUEST_SIZE];

    while (true) {
        memset(request, 0, MAX_REQUEST_SIZE);
        int bytes_received = recv(client_socket, request, MAX_REQUEST_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        printf("Client %d sent request: %s\n", client_socket, request);

        // Do something with the request here

        send(client_socket, "Response", strlen("Response"), 0);
    }

    close(client_socket);
    num_clients--;
}

void accept_connections() {
    while (num_clients < MAX_CLIENTS) {
        int listening_socket = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in server_addr;

        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(PORT);

        bind(listening_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
        listen(listening_socket, 3);

        printf("Listening on port %d\n", PORT);

        int client_socket = accept(listening_socket, NULL, NULL);
        client_sockets[num_clients] = client_socket;
        num_clients++;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
    }
}

int main() {
    accept_connections();

    while (true) {
        sleep(1);
    }

    return 0;
}