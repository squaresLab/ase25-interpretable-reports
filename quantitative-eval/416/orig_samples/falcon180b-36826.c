//Falcon-180B DATASET v1.0 Category: Internet Speed Test Application ; Style: multiplayer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define SERVER_PORT 8080

int client_sockets[MAX_CLIENTS];
int client_socket_count = 0;

void *handle_client(void *socket_desc) {
    int client_socket = *(int *) socket_desc;
    char buffer[BUFFER_SIZE];

    while(1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if(bytes_received <= 0) {
            break;
        }

        printf("Client %d sent: %s\n", client_socket, buffer);
        for(int i=0; i<client_socket_count; i++) {
            if(client_sockets[i]!= client_socket) {
                send(client_sockets[i], buffer, strlen(buffer), 0);
            }
        }
    }

    close(client_socket);
    return NULL;
}

void accept_clients() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);

    printf("Server is listening on port %d\n", SERVER_PORT);

    while(1) {
        int client_socket = accept(server_socket, NULL, NULL);
        if(client_socket == -1) {
            printf("Error accepting client\n");
            continue;
        }

        if(client_socket_count >= MAX_CLIENTS) {
            printf("Maximum clients reached\n");
            close(client_socket);
            continue;
        }

        client_sockets[client_socket_count] = client_socket;
        client_socket_count++;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, &client_socket);
    }
}

int main() {
    accept_clients();
    return 0;
}