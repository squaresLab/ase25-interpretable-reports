//Falcon-180B DATASET v1.0 Category: Chat server ; Style: Donald Knuth
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

int client_sockets[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *socket_desc) {
    char buffer[BUFFER_SIZE];
    int client_socket = *(int *) socket_desc;

    while(1) {
        bzero(buffer, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if(bytes_received <= 0)
            break;

        send(client_socket, buffer, bytes_received, 0);
    }

    close(client_socket);
    return NULL;
}

void broadcast(int sender_socket, char message[BUFFER_SIZE]) {
    for(int i = 0; i < num_clients; i++) {
        if(client_sockets[i]!= sender_socket) {
            send(client_sockets[i], message, strlen(message), 0);
        }
    }
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, 3);

    printf("Server is listening on port 8080\n");

    while(1) {
        int client_socket = accept(server_socket, NULL, NULL);

        if(num_clients >= MAX_CLIENTS) {
            send(client_socket, "Server is full. Please try again later.\n", 51, 0);
            close(client_socket);
        } else {
            client_sockets[num_clients] = client_socket;
            num_clients++;

            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket);
        }
    }

    return 0;
}