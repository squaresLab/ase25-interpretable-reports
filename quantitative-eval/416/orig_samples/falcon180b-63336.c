//Falcon-180B DATASET v1.0 Category: GPS Navigation Simulation ; Style: asynchronous
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define PORT 8080

int server_socket;
int client_sockets[MAX_CLIENTS];
int num_clients = 0;

void * handle_client(void * arg) {
    int client_socket = *(int *)arg;
    char buffer[1024];

    while(1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

        if(bytes_received <= 0) {
            close(client_socket);
            client_sockets[num_clients - 1] = -1;
            num_clients--;
            break;
        }

        printf("Client %d sent: %s\n", client_socket, buffer);
    }

    return NULL;
}

void accept_client() {
    struct sockaddr_in client_addr;
    socklen_t client_size = sizeof(client_addr);

    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_size);

    if(client_socket < 0) {
        printf("Error accepting client\n");
        return;
    }

    if(num_clients >= MAX_CLIENTS) {
        close(client_socket);
        printf("Server is full\n");
        return;
    }

    client_sockets[num_clients] = client_socket;
    num_clients++;

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket);
}

int main() {
    int opt = 1;
    struct sockaddr_in server_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(server_socket < 0) {
        printf("Error creating socket\n");
        return -1;
    }

    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if(bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error binding socket\n");
        return -1;
    }

    if(listen(server_socket, MAX_CLIENTS) < 0) {
        printf("Error listening on socket\n");
        return -1;
    }

    printf("Server started on port %d\n", PORT);

    while(1) {
        accept_client();
    }

    return 0;
}