//Falcon-180B DATASET v1.0 Category: Socket programming ; Style: multiplayer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int clients[MAX_CLIENTS][2]; // client sockets
int num_clients = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void broadcast(int sender_socket, char* message) {
    pthread_mutex_lock(&lock);
    int i;
    for (i = 0; i < num_clients; i++) {
        if (clients[i][0]!= sender_socket) {
            send(clients[i][0], message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&lock);
}

void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            break;
        }
        broadcast(client_socket, buffer);
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

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 3);

    printf("Server is running on port 8080\n");

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (num_clients >= MAX_CLIENTS) {
            send(client_socket, "Server is full", strlen("Server is full"), 0);
            close(client_socket);
        } else {
            int i;
            for (i = 0; i < num_clients; i++) {
                if (clients[i][0] == 0) {
                    clients[i][0] = client_socket;
                    clients[i][1] = i;
                    num_clients++;
                    break;
                }
            }
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, &client_socket);
        }
    }

    return 0;
}