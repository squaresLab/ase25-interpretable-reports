//Falcon-180B DATASET v1.0 Category: Chat server ; Style: surprised
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int g_server_socket;
int g_client_sockets[MAX_CLIENTS];
pthread_t g_client_threads[MAX_CLIENTS];
int g_num_clients = 0;

void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        char* message = strdup(buffer);
        printf("Client %d sent message: %s\n", client_socket, message);

        // Send message to all connected clients
        for (int i = 0; i < g_num_clients; i++) {
            if (g_client_sockets[i]!= client_socket) {
                send(g_client_sockets[i], message, strlen(message), 0);
            }
        }

        free(message);
    }

    close(client_socket);
    return NULL;
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 10);

    printf("Server started on port 8080\n");

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);

        if (g_num_clients >= MAX_CLIENTS) {
            send(client_socket, "Server is full\n", 14, 0);
            close(client_socket);
            continue;
        }

        g_client_sockets[g_num_clients] = client_socket;
        g_num_clients++;

        pthread_create(&g_client_threads[g_num_clients - 1], NULL, handle_client, (void*)&client_socket);
    }

    return 0;
}