//Falcon-180B DATASET v1.0 Category: Interprocess communication ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUF_SIZE 1024

int client_sockets[MAX_CLIENTS];
int num_clients = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void broadcast(int socket, char *message) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < num_clients; i++) {
        if (client_sockets[i]!= socket) {
            send(client_sockets[i], message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&lock);
}

void *handle_client(void *socket_desc) {
    int sock = *(int *) socket_desc;
    char buffer[BUF_SIZE];

    while (1) {
        memset(buffer, 0, BUF_SIZE);
        int bytes_received = recv(sock, buffer, BUF_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        broadcast(sock, buffer);
    }

    close(sock);
    return NULL;
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(server_socket, 3);

    printf("Server is running on port 8080\n");

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);

        pthread_mutex_lock(&lock);
        for (int i = 0; i < num_clients; i++) {
            if (client_sockets[i] == 0) {
                client_sockets[i] = client_socket;
                num_clients++;
                break;
            }
        }
        pthread_mutex_unlock(&lock);

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
    }

    return 0;
}