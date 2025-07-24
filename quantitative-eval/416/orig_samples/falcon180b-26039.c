//Falcon-180B DATASET v1.0 Category: Smart home light control ; Style: happy
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define PORT 8080
#define BUFFER_SIZE 1024

int server_socket;
struct sockaddr_in server_addr;
int client_sockets[MAX_CLIENTS];
int num_clients = 0;

char *message = "Hello from the server!";

void *handle_client(void *client_socket) {
    int client = *(int *) client_socket;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        printf("Client %d sent: %s\n", client, buffer);
    }

    close(client);
    num_clients--;
}

int main() {
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);

    printf("Server started on port %d\n", PORT);

    while (1) {
        socklen_t client_size = sizeof(client_sockets[num_clients]);
        int client = accept(server_socket, (struct sockaddr *) &client_sockets[num_clients], &client_size);

        if (client < 0) {
            printf("Error accepting connection\n");
            continue;
        }

        num_clients++;
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client);
    }

    return 0;
}