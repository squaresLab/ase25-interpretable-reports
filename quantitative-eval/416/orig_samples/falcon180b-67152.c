//Falcon-180B DATASET v1.0 Category: Chat server ; Style: brave
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define MAX_MESSAGE_LENGTH 100

int clients[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char message[MAX_MESSAGE_LENGTH];

    while (1) {
        memset(message, 0, MAX_MESSAGE_LENGTH);
        int bytes_received = recv(client_socket, message, MAX_MESSAGE_LENGTH, 0);

        if (bytes_received <= 0) {
            printf("Client %d disconnected.\n", client_socket);
            close(client_socket);
            return NULL;
        }

        printf("Client %d: %s", client_socket, message);
        send(client_socket, message, strlen(message), 0);
    }
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);

    printf("Server is running on port 8080.\n");

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        int i;

        for (i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] == 0) {
                clients[i] = client_socket;
                num_clients++;
                pthread_t thread_id;
                pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
                break;
            }
        }
    }

    return 0;
}