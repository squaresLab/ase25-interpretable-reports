//Falcon-180B DATASET v1.0 Category: Chat server ; Style: decentralized
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define MAX_MESSAGE_LENGTH 1024

int clients[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *socket_desc);

int main(int argc, char **argv) {
    int server_socket;
    struct sockaddr_in server_addr;
    int opt = 1;
    int addrlen = sizeof(server_addr);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set options on the socket
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Bind the socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server is running on port 8080\n");

    while (1) {
        socklen_t client_len;
        int client_socket = accept(server_socket, (struct sockaddr *)&server_addr, &client_len);

        if (client_socket < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] == 0) {
                clients[i] = client_socket;
                num_clients++;
                break;
            }
        }

        if (num_clients == MAX_CLIENTS) {
            close(client_socket);
            printf("Server is full. Please try again later.\n");
        } else {
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket);
        }
    }

    return 0;
}

void *handle_client(void *socket_desc) {
    int sock = *((int *) socket_desc);
    char buffer[MAX_MESSAGE_LENGTH];

    while (1) {
        memset(buffer, 0, MAX_MESSAGE_LENGTH);
        int bytes_received = recv(sock, buffer, MAX_MESSAGE_LENGTH, 0);

        if (bytes_received <= 0) {
            close(sock);
            pthread_exit(NULL);
        }

        send(sock, buffer, strlen(buffer), 0);
    }
}