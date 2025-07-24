//Falcon-180B DATASET v1.0 Category: Music Library Management System ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10
#define PORT 8080

int client_sockets[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *socket_desc) {
    int client_socket = *(int *) socket_desc;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        printf("Client %d sent: %s\n", client_socket, buffer);
    }

    close(client_socket);
    return NULL;
}

void accept_connections() {
    while (1) {
        int server_socket = socket(AF_INET, SOCK_STREAM, 0);

        if (server_socket == -1) {
            printf("Error creating socket\n");
            exit(1);
        }

        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
        server_addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
            printf("Error binding socket\n");
            exit(1);
        }

        if (listen(server_socket, 3) == -1) {
            printf("Error listening on socket\n");
            exit(1);
        }

        printf("Server is listening on port %d\n", PORT);

        while (num_clients < MAX_CLIENTS) {
            socklen_t addr_size = sizeof(struct sockaddr_in);
            int client_socket = accept(server_socket, (struct sockaddr *) &client_sockets[num_clients], &addr_size);

            if (client_socket == -1) {
                printf("Error accepting connection\n");
            } else {
                num_clients++;
                pthread_t thread_id;
                pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
            }
        }
    }
}

int main() {
    srand(time(NULL));

    accept_connections();

    return 0;
}