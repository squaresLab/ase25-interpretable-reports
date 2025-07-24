//Falcon-180B DATASET v1.0 Category: Database Indexing System ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define MAX_DATA_SIZE 100

int clients[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *socket) {
    int client_socket = *(int *)socket;
    char data[MAX_DATA_SIZE];
    int bytes_received;

    while ((bytes_received = recv(client_socket, data, MAX_DATA_SIZE, 0)) > 0) {
        printf("Client %d sent data: %s\n", client_socket, data);
    }

    close(client_socket);
}

int main() {
    int server_socket;
    int opt = 1;
    struct sockaddr_in server_addr;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        printf("Error creating socket\n");
        exit(1);
    }

    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error binding socket\n");
        exit(1);
    }

    if (listen(server_socket, MAX_CLIENTS) < 0) {
        printf("Error listening on socket\n");
        exit(1);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        int client_socket;
        socklen_t client_size;

        if ((client_socket = accept(server_socket, (struct sockaddr *)&server_addr, &client_size)) < 0) {
            printf("Error accepting client connection\n");
            exit(1);
        }

        if (num_clients >= MAX_CLIENTS) {
            printf("Maximum clients reached\n");
            close(client_socket);
            continue;
        }

        clients[num_clients] = client_socket;
        num_clients++;

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket) < 0) {
            printf("Error creating thread\n");
            exit(1);
        }
    }

    return 0;
}