//Falcon-180B DATASET v1.0 Category: Chat server ; Style: visionary
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

int clients[MAX_CLIENTS];
int num_clients = 0;
int main_socket;

void *handle_client(void *client_socket) {
    int client_socket_copy = *((int *) client_socket);
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket_copy, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            break;
        }
        send(client_socket_copy, buffer, bytes_received, 0);
    }
    close(client_socket_copy);
    return NULL;
}

void accept_connections() {
    while (1) {
        int client_socket = accept(main_socket, NULL, NULL);
        if (client_socket == -1) {
            continue;
        }
        clients[num_clients] = client_socket;
        num_clients++;
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
    }
}

int main() {
    int opt = 1;
    setsockopt(main_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(main_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(main_socket, 10);

    accept_connections();

    return 0;
}