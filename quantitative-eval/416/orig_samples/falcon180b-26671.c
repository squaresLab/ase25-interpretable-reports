//Falcon-180B DATASET v1.0 Category: Chat server ; Style: Cryptic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define MAX_MESSAGE_LENGTH 1024
#define PORT 8080

int client_sockets[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[MAX_MESSAGE_LENGTH];

    while (1) {
        memset(buffer, 0, MAX_MESSAGE_LENGTH);
        int bytes_received = recv(client_socket, buffer, MAX_MESSAGE_LENGTH, 0);

        if (bytes_received <= 0) {
            close(client_socket);
            pthread_exit(0);
        }

        char *message = strtok(buffer, " ");
        send(client_socket, message, strlen(message), 0);
    }
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(server_socket, 3);

    printf("Server started on port %d\n", PORT);

    while (1) {
        socklen_t client_size = sizeof(client_sockets[num_clients]);
        int client_socket = accept(server_socket, (struct sockaddr *) &client_sockets[num_clients], &client_size);

        if (num_clients >= MAX_CLIENTS) {
            close(client_socket);
            printf("Maximum clients reached\n");
        } else {
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
            num_clients++;
        }
    }

    return 0;
}