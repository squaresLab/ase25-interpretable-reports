//Falcon-180B DATASET v1.0 Category: Chat server ; Style: scalable
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAX_CLIENTS 100
#define MAX_MESSAGE_SIZE 1024

int clients[MAX_CLIENTS];

void *thread_handler(void *arg) {
    int client_socket = *(int *) arg;
    char message[MAX_MESSAGE_SIZE];

    while (1) {
        memset(message, 0, MAX_MESSAGE_SIZE);
        int bytes_received = recv(client_socket, message, MAX_MESSAGE_SIZE, 0);

        if (bytes_received <= 0) {
            close(client_socket);
            break;
        }

        send(client_socket, message, bytes_received, 0);
    }

    return NULL;
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1) {
        printf("Error creating socket\n");
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        printf("Error binding socket\n");
        return 2;
    }

    if (listen(server_socket, MAX_CLIENTS) == -1) {
        printf("Error listening on socket\n");
        return 3;
    }

    printf("Server started on port 8080\n");

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);

        if (client_socket == -1) {
            printf("Error accepting connection\n");
            continue;
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i] == 0) {
                clients[i] = client_socket;
                pthread_t thread_id;
                pthread_create(&thread_id, NULL, thread_handler, (void *) &client_socket);
                break;
            }
        }
    }

    close(server_socket);
    return 0;
}