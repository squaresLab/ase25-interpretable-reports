//Falcon-180B DATASET v1.0 Category: Simple HTTP Proxy ; Style: Claude Shannon
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/time.h>
#include <pthread.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

struct client {
    int socket;
    char *ip;
    int port;
};

struct client clients[MAX_CLIENTS];
int num_clients = 0;

void *handle_request(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            break;
        }
        for (int i = 0; i < num_clients; i++) {
            if (clients[i].socket!= client_socket) {
                send(clients[i].socket, buffer, bytes_received, 0);
            }
        }
    }
    close(client_socket);
    num_clients--;
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "0.0.0.0", &server_addr.sin_addr);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, 10);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size);
        if (num_clients >= MAX_CLIENTS) {
            send(client_socket, "Server is full\n", 15, 0);
            close(client_socket);
            continue;
        }
        clients[num_clients].socket = client_socket;
        clients[num_clients].ip = inet_ntoa(client_addr.sin_addr);
        clients[num_clients].port = ntohs(client_addr.sin_port);
        num_clients++;
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_request, (void *)&client_socket);
    }

    return 0;
}