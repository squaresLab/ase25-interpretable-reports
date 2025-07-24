//Falcon-180B DATASET v1.0 Category: Robot movement control ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int g_socket = -1;
int g_clients[MAX_CLIENTS] = {0};
int g_num_clients = 0;

void *handle_client(void *param) {
    int client_socket = *(int *)param;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        printf("Client %d: %s\n", client_socket, buffer);
    }

    close(client_socket);
    return NULL;
}

void accept_connections() {
    while (1) {
        int client_socket = accept(g_socket, NULL, NULL);

        if (g_num_clients >= MAX_CLIENTS) {
            send(client_socket, "Server is full.\n", 15, 0);
            close(client_socket);
            continue;
        }

        g_clients[g_num_clients++] = client_socket;
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket);
    }
}

int main() {
    g_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (g_socket == -1) {
        printf("Error creating socket.\n");
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(g_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("Error binding socket.\n");
        return 1;
    }

    if (listen(g_socket, MAX_CLIENTS) == -1) {
        printf("Error listening on socket.\n");
        return 1;
    }

    printf("Server started on port %d.\n", PORT);
    accept_connections();

    return 0;
}