//Falcon-180B DATASET v1.0 Category: Socket programming ; Style: complex
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

int server_socket;
int client_sockets[MAX_CLIENTS];
int num_clients = 0;
pthread_t threads[MAX_CLIENTS];

void *handle_client(void *socket_desc) {
    int sock = *(int *) socket_desc;
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            close(sock);
            pthread_exit(0);
        }
        printf("Client %d sent: %s\n", sock, buffer);
        send(sock, buffer, strlen(buffer), 0);
    }
}

int main() {
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8080);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, 3);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_size = sizeof(client_addr);
        int new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);

        if (num_clients >= MAX_CLIENTS) {
            send(new_socket, "Server is full.\n", 16, 0);
            close(new_socket);
        } else {
            client_sockets[num_clients] = new_socket;
            pthread_create(&threads[num_clients], NULL, handle_client, (void *)&new_socket);
            num_clients++;
        }
    }

    return 0;
}