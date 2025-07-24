//Falcon-180B DATASET v1.0 Category: Chat server ; Style: puzzling
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>

#define MAX_CLIENTS 50
#define MAX_MSG_SIZE 1024

int client_sockets[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *socket_desc) {
    char recv_msg[MAX_MSG_SIZE];
    int client_socket = *(int *)socket_desc;

    while (1) {
        memset(recv_msg, 0, MAX_MSG_SIZE);
        int bytes_received = recv(client_socket, recv_msg, MAX_MSG_SIZE, 0);

        if (bytes_received <= 0) {
            close(client_socket);
            pthread_exit(0);
        }

        recv_msg[bytes_received] = '\0';
        printf("Client %d sent message: %s\n", client_socket, recv_msg);

        send(client_socket, recv_msg, strlen(recv_msg), 0);
    }
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(3000);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);

    printf("Server is listening on port 3000\n");

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);

        if (client_socket == -1) {
            printf("Error accepting connection\n");
            continue;
        }

        num_clients++;

        if (num_clients >= MAX_CLIENTS) {
            close(client_socket);
            printf("Server is full. Cannot accept more connections.\n");
            continue;
        }

        client_sockets[num_clients - 1] = client_socket;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket);
    }

    return 0;
}