//Falcon-180B DATASET v1.0 Category: Client Server Application ; Style: scientific
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 8080
#define BACKLOG 10
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

int server_socket;
int client_sockets[MAX_CLIENTS];
int num_clients = 0;
pthread_t threads[MAX_CLIENTS];

void *handle_client(void *socket_desc) {
    int client_socket = *(int *) socket_desc;
    char buffer[BUFFER_SIZE];

    while(1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if(bytes_received <= 0) {
            if(bytes_received == 0)
                printf("Client %d disconnected\n", client_socket);
            else
                printf("Client %d connection error\n", client_socket);

            close(client_socket);
            pthread_exit(0);
        }

        printf("Client %d sent: %s\n", client_socket, buffer);
        send(client_socket, buffer, strlen(buffer), 0);
    }
}

int main(int argc, char *argv[]) {
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, BACKLOG);

    printf("Server listening on port %d\n", PORT);

    while(1) {
        socklen_t client_size = sizeof(struct sockaddr_in);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_sockets[num_clients], &client_size);

        if(client_socket == -1) {
            printf("Error accepting connection\n");
            continue;
        }

        num_clients++;
        printf("Client %d connected\n", client_socket);

        pthread_create(&threads[num_clients - 1], NULL, handle_client, (void *)&client_socket);
    }

    return 0;
}