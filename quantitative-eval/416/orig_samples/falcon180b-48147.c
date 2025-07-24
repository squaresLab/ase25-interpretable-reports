//Falcon-180B DATASET v1.0 Category: System event logger ; Style: decentralized
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BACKLOG 10
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int g_server_socket;
int g_client_sockets[MAX_CLIENTS];
int g_num_clients = 0;
pthread_t g_threads[MAX_CLIENTS];

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[BUFFER_SIZE];

    while (1) {
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        printf("Client %d sent: %s\n", client_socket, buffer);
    }

    close(client_socket);
    pthread_exit(NULL);
}

void accept_client(int server_socket) {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_socket;

    client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &addr_len);

    if (client_socket < 0) {
        printf("Error accepting client\n");
        return;
    }

    if (g_num_clients >= MAX_CLIENTS) {
        printf("Maximum number of clients reached\n");
        close(client_socket);
        return;
    }

    g_client_sockets[g_num_clients] = client_socket;
    g_num_clients++;

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0) {
        printf("Error creating socket\n");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        printf("Error binding socket\n");
        return 2;
    }

    if (listen(server_socket, BACKLOG) < 0) {
        printf("Error listening on socket\n");
        return 3;
    }

    printf("Server started on port %d\n", PORT);

    while (1) {
        accept_client(server_socket);
    }

    return 0;
}