//Falcon-180B DATASET v1.0 Category: Networking ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BACKLOG 10
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int clients[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *socket_desc) {
    int sock = *(int *) socket_desc;
    char buffer[BUFFER_SIZE];

    while(1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);

        if(bytes_received <= 0) {
            break;
        }

        printf("Client %d sent: %s\n", sock, buffer);

        for(int i = 0; i < num_clients; i++) {
            if(clients[i]!= sock) {
                send(clients[i], buffer, strlen(buffer), 0);
            }
        }
    }

    close(sock);

    return NULL;
}

int main(int argc, char *argv[]) {
    int master_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(master_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));

    listen(master_socket, BACKLOG);

    printf("Server is listening on port %d\n", PORT);

    while(1) {
        int client_socket = accept(master_socket, NULL, NULL);

        if(num_clients >= MAX_CLIENTS) {
            send(client_socket, "Server is full. Please try again later.\n", 60, 0);
            close(client_socket);
            continue;
        }

        clients[num_clients] = client_socket;
        num_clients++;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
    }

    return 0;
}