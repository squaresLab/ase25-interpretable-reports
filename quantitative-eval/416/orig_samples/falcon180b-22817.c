//Falcon-180B DATASET v1.0 Category: Smart home light control ; Style: imaginative
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define TRUE   1
#define FALSE  0
#define DEBUG  TRUE

#define MAX_CONNECTIONS 10
#define BACKLOG 10
#define MAX_DATA_SIZE 1024

int listening_socket;
int client_sockets[MAX_CONNECTIONS];
int num_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[MAX_DATA_SIZE];

    while (TRUE) {
        memset(buffer, 0, MAX_DATA_SIZE);
        int bytes_received = recv(client_socket, buffer, MAX_DATA_SIZE, 0);

        if (bytes_received <= 0) {
            close(client_socket);
            client_socket = -1;
            break;
        }

        printf("Client %d sent: %s\n", client_socket, buffer);

        for (int i = 0; i < num_clients; i++) {
            if (client_sockets[i]!= client_socket) {
                send(client_sockets[i], buffer, strlen(buffer), 0);
            }
        }
    }

    return NULL;
}

int main() {
    int opt = TRUE;
    setsockopt(listening_socket, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt));

    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    bind(listening_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(listening_socket, BACKLOG);

    printf("Server is listening on port 8080...\n");

    while (TRUE) {
        int client_socket = accept(listening_socket, (struct sockaddr *) &client_addr, sizeof(client_addr));

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
        pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
    }

    close(listening_socket);
    return 0;
}