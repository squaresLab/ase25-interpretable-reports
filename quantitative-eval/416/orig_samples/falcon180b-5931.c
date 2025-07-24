//Falcon-180B DATASET v1.0 Category: Socket programming ; Style: recursive
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CONNECTIONS 10
#define MAX_MSG_SIZE 1024

int client_sockets[MAX_CONNECTIONS];
int num_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[MAX_MSG_SIZE];
    printf("Client %d connected\n", client_socket);

    // Receive message from client
    memset(buffer, 0, MAX_MSG_SIZE);
    int bytes_received = recv(client_socket, buffer, MAX_MSG_SIZE, 0);
    if (bytes_received <= 0) {
        printf("Client %d disconnected\n", client_socket);
        close(client_socket);
        return NULL;
    }
    printf("Received message from client %d: %s\n", client_socket, buffer);

    // Send message to all clients
    for (int i = 0; i < num_clients; i++) {
        if (client_socket!= client_sockets[i]) {
            send(client_sockets[i], buffer, strlen(buffer), 0);
        }
    }

    close(client_socket);
    return NULL;
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Error creating socket\n");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        printf("Error binding socket\n");
        exit(1);
    }

    if (listen(server_socket, MAX_CONNECTIONS) == -1) {
        printf("Error listening on socket\n");
        exit(1);
    }

    printf("Server started on port 8080\n");

    while (1) {
        socklen_t socklen = sizeof(struct sockaddr_in);
        int client_socket = accept(server_socket, (struct sockaddr *) &server_addr, &socklen);
        if (client_socket == -1) {
            printf("Error accepting connection\n");
            continue;
        }

        if (num_clients >= MAX_CONNECTIONS) {
            close(client_socket);
            continue;
        }

        client_sockets[num_clients++] = client_socket;
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
    }

    return 0;
}