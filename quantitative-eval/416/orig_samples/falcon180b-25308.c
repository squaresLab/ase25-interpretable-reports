//Falcon-180B DATASET v1.0 Category: Browser Plugin ; Style: multiplayer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int g_client_sockets[MAX_CLIENTS] = {0};
int g_client_count = 0;

void* handle_client(void* arg) {
    int client_socket = *(int*) arg;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        printf("Client %d sent: %s\n", client_socket, buffer);
        for (int i = 0; i < g_client_count; i++) {
            if (g_client_sockets[i]!= client_socket) {
                send(g_client_sockets[i], buffer, bytes_received, 0);
            }
        }
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

    bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);

    printf("Server started on port 8080\n");

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (g_client_sockets[i] == 0) {
                g_client_sockets[i] = client_socket;
                g_client_count++;
                printf("Client %d connected\n", client_socket);
                pthread_t thread_id;
                pthread_create(&thread_id, NULL, handle_client, (void*) &client_socket);
                break;
            }
        }
    }

    close(server_socket);
    return 0;
}