//Falcon-180B DATASET v1.0 Category: Chat server ; Style: synchronous
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUF_SIZE 1024

int clients[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *client_socket) {
    int client_sock = *(int *) client_socket;
    char buf[BUF_SIZE];

    while (1) {
        memset(buf, 0, BUF_SIZE);
        int bytes_received = recv(client_sock, buf, BUF_SIZE, 0);

        if (bytes_received <= 0) {
            close(client_sock);
            clients[num_clients--] = -1;
            break;
        }

        for (int i = 0; i < num_clients; i++) {
            if (clients[i]!= -1) {
                send(clients[i], buf, bytes_received, 0);
            }
        }
    }

    return NULL;
}

int main() {
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (server_sock == -1) {
        printf("Error creating socket\n");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        printf("Error binding socket\n");
        exit(1);
    }

    if (listen(server_sock, MAX_CLIENTS) == -1) {
        printf("Error listening on socket\n");
        exit(1);
    }

    printf("Server started on port 8080\n");

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_size = sizeof(client_addr);
        int client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_size);

        if (client_sock!= -1) {
            clients[num_clients++] = client_sock;
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, (void *) &client_sock);
        }
    }

    return 0;
}