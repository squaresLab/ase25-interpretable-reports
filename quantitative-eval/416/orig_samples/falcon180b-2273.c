//Falcon-180B DATASET v1.0 Category: Chat server ; Style: scientific
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define MAX_MSG_SIZE 1024

int clients[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[MAX_MSG_SIZE];
    while (1) {
        memset(buffer, 0, MAX_MSG_SIZE);
        if (recv(client_socket, buffer, MAX_MSG_SIZE, 0) <= 0) {
            close(client_socket);
            clients[num_clients - 1] = 0;
            num_clients--;
            break;
        }
        for (int i = 0; i < num_clients; i++) {
            if (clients[i]!= client_socket) {
                send(clients[i], buffer, strlen(buffer), 0);
            }
        }
    }
    pthread_exit(NULL);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port 8080\n");
    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            perror("accept");
            continue;
        }
        if (num_clients == MAX_CLIENTS) {
            close(client_socket);
            printf("Server is full\n");
        } else {
            clients[num_clients] = client_socket;
            num_clients++;
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
        }
    }
    close(server_socket);
    return 0;
}