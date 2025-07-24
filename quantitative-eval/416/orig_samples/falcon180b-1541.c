//Falcon-180B DATASET v1.0 Category: Chat server ; Style: minimalist
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int g_socket;
int g_client_sockets[MAX_CLIENTS];
int g_num_clients = 0;

void *thread_func(void *param) {
    int client_socket = *(int*)param;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            close(client_socket);
            return NULL;
        }

        for (int i = 0; i < g_num_clients; i++) {
            if (g_client_sockets[i]!= client_socket) {
                send(g_client_sockets[i], buffer, bytes_received, 0);
            }
        }
    }

    return NULL;
}

void accept_connections() {
    while (g_num_clients < MAX_CLIENTS) {
        int client_socket = accept(g_socket, NULL, NULL);

        if (client_socket == -1) {
            printf("Error accepting connection\n");
            continue;
        }

        g_client_sockets[g_num_clients] = client_socket;
        g_num_clients++;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, thread_func, (void*)&client_socket);
    }
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1) {
        printf("Error creating socket\n");
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        printf("Error binding socket\n");
        return 1;
    }

    if (listen(server_socket, MAX_CLIENTS) == -1) {
        printf("Error listening on socket\n");
        return 1;
    }

    printf("Server started on port 8080\n");

    accept_connections();

    return 0;
}