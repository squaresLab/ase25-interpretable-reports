//Falcon-180B DATASET v1.0 Category: Client Server Application ; Style: surrealist
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define LISTEN_PORT 8080

int listen_sock = 0;
int client_socks[MAX_CLIENTS] = { 0 };
int num_clients = 0;

void *thread_client(void *arg) {
    int sock = *(int *) arg;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        recv(sock, buffer, BUFFER_SIZE, 0);
        printf("Client %d: %s\n", sock, buffer);
        send(sock, buffer, strlen(buffer), 0);
    }

    close(sock);
    pthread_exit(0);
}

int main() {
    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(LISTEN_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(listen_sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(listen_sock, 5);

    printf("Server is listening on port %d\n", LISTEN_PORT);

    while (1) {
        int client_sock = accept(listen_sock, NULL, NULL);

        if (num_clients >= MAX_CLIENTS) {
            send(client_sock, "Server is full. Please try again later.\n", 45, 0);
            close(client_sock);
        } else {
            client_socks[num_clients++] = client_sock;
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, thread_client, (void *) &client_sock);
        }
    }

    close(listen_sock);
    return 0;
}