//Falcon-180B DATASET v1.0 Category: Subnet Calculator ; Style: decentralized
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int clients[MAX_CLIENTS];
int num_clients = 0;
int server_socket;

void *handle_client(void *client_socket) {
    int client = *(int *) client_socket;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            close(client);
            pthread_exit(NULL);
        }

        printf("Client %d sent message: %s\n", client, buffer);
    }
}

void accept_clients() {
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_size = sizeof(client_addr);
        int client = accept(server_socket, (struct sockaddr *) &client_addr, &addr_size);

        if (client == -1) {
            printf("Error accepting client\n");
            continue;
        }

        printf("Client %d connected\n", client);

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client);
        num_clients++;
    }
}

int main() {
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "0.0.0.0", &server_addr.sin_addr);

    bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);

    printf("Server listening on port 8080\n");

    accept_clients();

    return 0;
}