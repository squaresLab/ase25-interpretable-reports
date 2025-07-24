//Falcon-180B DATASET v1.0 Category: TCP/IP Programming ; Style: single-threaded
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BACKLOG 10
#define MAX_CLIENT 10
#define BUFFER_SIZE 1024

int clients[MAX_CLIENT];
int client_count = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        printf("Client %d sent: %s\n", client_socket, buffer);

        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
    return NULL;
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Socket creation failed\n");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        printf("Bind failed\n");
        exit(1);
    }

    if (listen(server_socket, BACKLOG) == -1) {
        printf("Listen failed\n");
        exit(1);
    }

    while (1) {
        int client_socket;
        socklen_t client_size;

        if ((client_socket = accept(server_socket, (struct sockaddr *) &server_addr, &client_size)) == -1) {
            printf("Accept failed\n");
            exit(1);
        }

        int i;
        for (i = 0; i < MAX_CLIENT; i++) {
            if (clients[i] == 0) {
                clients[i] = client_socket;
                client_count++;
                break;
            }
        }

        if (i == MAX_CLIENT) {
            printf("Maximum number of clients reached\n");
            close(client_socket);
            continue;
        }

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
    }

    close(server_socket);
    return 0;
}