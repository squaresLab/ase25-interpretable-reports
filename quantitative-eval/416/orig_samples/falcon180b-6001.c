//Falcon-180B DATASET v1.0 Category: Smart home automation ; Style: multiplayer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int server_socket;
int client_sockets[MAX_CLIENTS];
int client_count = 0;
char send_buffer[BUFFER_SIZE];
char receive_buffer[BUFFER_SIZE];

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    printf("Client %d connected\n", client_socket);

    while (1) {
        memset(send_buffer, 0, BUFFER_SIZE);
        memset(receive_buffer, 0, BUFFER_SIZE);

        int bytes_received = recv(client_socket, receive_buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            printf("Client %d disconnected\n", client_socket);
            close(client_socket);
            return NULL;
        }

        send_buffer[0] = 'E';
        send_buffer[1] = 'C';
        send_buffer[2] = 'H';
        send_buffer[3] = 'O';

        int bytes_sent = send(client_socket, send_buffer, strlen(send_buffer), 0);

        if (bytes_sent <= 0) {
            printf("Client %d disconnected\n", client_socket);
            close(client_socket);
            return NULL;
        }
    }
}

int main() {
    int opt = 1;
    int addrlen = sizeof(struct sockaddr_in);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Error creating socket\n");
        return 1;
    }

    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, addrlen) == -1) {
        printf("Error binding socket\n");
        return 1;
    }

    if (listen(server_socket, MAX_CLIENTS) == -1) {
        printf("Error listening on socket\n");
        return 1;
    }

    printf("Server started on port 8080\n");

    while (1) {
        int client_socket = accept(server_socket, (struct sockaddr *)NULL, NULL);
        if (client_socket == -1) {
            printf("Error accepting connection\n");
            continue;
        }

        if (client_count >= MAX_CLIENTS) {
            printf("Maximum clients reached\n");
            close(client_socket);
            continue;
        }

        client_sockets[client_count++] = client_socket;
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket);
    }

    return 0;
}