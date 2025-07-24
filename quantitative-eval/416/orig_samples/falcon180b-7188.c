//Falcon-180B DATASET v1.0 Category: Smart home light control ; Style: Dennis Ritchie
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8888
#define BACKLOG 10
#define MAX_CLIENTS 10
#define MAX_REQUEST_SIZE 1024

int clients[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[MAX_REQUEST_SIZE];

    while (1) {
        memset(buffer, 0, MAX_REQUEST_SIZE);
        int bytes_received = recv(client_socket, buffer, MAX_REQUEST_SIZE, 0);

        if (bytes_received <= 0) {
            close(client_socket);
            pthread_exit(0);
        }

        printf("Request received from client %d: %s\n", client_socket, buffer);

        if (strcmp(buffer, "turn on") == 0) {
            system("echo 'Turning on lights'");
        } else if (strcmp(buffer, "turn off") == 0) {
            system("echo 'Turning off lights'");
        } else {
            send(client_socket, "Invalid request", strlen("Invalid request"), 0);
        }
    }
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, BACKLOG);

    printf("Server started on port %d\n", PORT);

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);

        if (num_clients >= MAX_CLIENTS) {
            send(client_socket, "Server full", strlen("Server full"), 0);
            close(client_socket);
        } else {
            clients[num_clients] = client_socket;
            num_clients++;

            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket);
        }
    }

    return 0;
}