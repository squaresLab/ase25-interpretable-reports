//Falcon-180B DATASET v1.0 Category: Firewall ; Style: Cryptic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define BACKLOG 5

int clients[MAX_CLIENTS];
int num_clients = 0;
int main_socket;
struct sockaddr_in server_addr;

void *thread_func(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            close(client_socket);
            return NULL;
        }
        printf("Client %d sent: %s\n", client_socket, buffer);
        for (int i = 0; i < num_clients; i++) {
            if (clients[i]!= client_socket) {
                send(clients[i], buffer, bytes_received, 0);
            }
        }
    }
}

int main() {
    int opt = 1;
    setsockopt(main_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(12345);

    bind(main_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(main_socket, BACKLOG);

    int thread_id;
    pthread_t thread[MAX_CLIENTS];

    while (1) {
        socklen_t addr_size = sizeof(struct sockaddr_in);
        int client_socket = accept(main_socket, (struct sockaddr *) &server_addr, &addr_size);

        if (num_clients >= MAX_CLIENTS) {
            close(client_socket);
            continue;
        }

        clients[num_clients] = client_socket;
        num_clients++;

        pthread_create(&thread[num_clients - 1], NULL, thread_func, (void *) &client_socket);
    }

    return 0;
}