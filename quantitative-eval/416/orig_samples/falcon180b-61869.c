//Falcon-180B DATASET v1.0 Category: Simple HTTP Proxy ; Style: invasive
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define MAX_CONNECTIONS 10

int listen_socket;
int client_sockets[MAX_CONNECTIONS];
int num_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[BUF_SIZE];

    while (1) {
        bzero(buffer, BUF_SIZE);
        int bytes_received = recv(client_socket, buffer, BUF_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        send(client_socket, buffer, bytes_received, 0);
    }

    close(client_socket);
    pthread_exit(0);
}

void accept_connections() {
    while (1) {
        int client_socket = accept(listen_socket, NULL, NULL);

        if (client_socket == -1) {
            printf("Error accepting connection\n");
            continue;
        }

        if (num_clients >= MAX_CONNECTIONS) {
            close(client_socket);
            printf("Maximum number of clients reached\n");
            continue;
        }

        client_sockets[num_clients] = client_socket;
        num_clients++;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, &client_socket);
    }
}

int main() {
    int opt = 1;
    int port = 8080;

    setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listen_socket, (struct sockaddr *) &server_addr, sizeof(server_addr))!= 0) {
        printf("Error binding socket\n");
        return 1;
    }

    if (listen(listen_socket, MAX_CONNECTIONS)!= 0) {
        printf("Error listening on socket\n");
        return 1;
    }

    accept_connections();

    return 0;
}