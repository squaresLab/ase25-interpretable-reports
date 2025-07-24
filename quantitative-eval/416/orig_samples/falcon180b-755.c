//Falcon-180B DATASET v1.0 Category: Medical Store Management System ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int clients[MAX_CLIENTS];
int num_clients = 0;
int main_socket = -1;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        printf("Client %d sent message: %s\n", client_socket, buffer);
    }

    close(client_socket);
    return NULL;
}

void add_client(int client_socket) {
    if (num_clients >= MAX_CLIENTS) {
        printf("Maximum number of clients reached.\n");
        return;
    }

    clients[num_clients] = client_socket;
    num_clients++;

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
}

int main() {
    int main_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (main_socket == -1) {
        printf("Error creating socket.\n");
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(main_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        printf("Error binding socket.\n");
        return 1;
    }

    if (listen(main_socket, 10) == -1) {
        printf("Error listening on socket.\n");
        return 1;
    }

    printf("Server started on port 8080.\n");

    while (1) {
        int client_socket = accept(main_socket, NULL, NULL);

        if (client_socket == -1) {
            printf("Error accepting connection.\n");
            continue;
        }

        add_client(client_socket);
    }

    close(main_socket);
    return 0;
}