//Falcon-180B DATASET v1.0 Category: Chat server ; Style: thoughtful
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 512

int clients[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *socket_desc) {
    int client_socket = *(int *) socket_desc;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            close(client_socket);
            clients[num_clients] = -1;
            num_clients--;
            break;
        }

        for (int i = 0; i < num_clients; i++) {
            if (clients[i]!= -1) {
                send(clients[i], buffer, bytes_received, 0);
            }
        }
    }

    return NULL;
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(5000);

    bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));

    listen(server_socket, 10);

    printf("Server listening on port 5000\n");

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);

        if (num_clients == MAX_CLIENTS) {
            close(client_socket);
            printf("Server is full\n");
        } else {
            clients[num_clients] = client_socket;
            num_clients++;

            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);

            printf("Client connected, total clients: %d\n", num_clients);
        }
    }

    return 0;
}