//Falcon-180B DATASET v1.0 Category: Email Client ; Style: puzzling
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define MAX_LINE 1024
#define SERVER_PORT 8080

int client_sockets[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[MAX_LINE];

    while (1) {
        memset(buffer, 0, MAX_LINE);
        int bytes_received = recv(client_socket, buffer, MAX_LINE, 0);
        if (bytes_received <= 0) {
            close(client_socket);
            num_clients--;
            break;
        }

        fprintf(stdout, "Client %d: %s\n", client_socket, buffer);

        send(client_socket, buffer, strlen(buffer), 0);
    }

    return NULL;
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            fprintf(stderr, "Error accepting client connection\n");
            continue;
        }

        num_clients++;
        fprintf(stdout, "Client %d connected\n", client_socket);

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
    }

    close(server_socket);
    return 0;
}