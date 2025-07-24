//Falcon-180B DATASET v1.0 Category: Network Packet Monitoring ; Style: asynchronous
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int g_client_socks[MAX_CLIENTS];
int g_num_clients = 0;

void *handle_client(void *arg) {
    int client_sock = *(int *) arg;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_sock, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        printf("Client %d sent: %s\n", client_sock, buffer);
    }

    close(client_sock);
    g_client_socks[g_num_clients - 1] = -1;
    g_num_clients--;

    pthread_exit(NULL);
}

void accept_connections() {
    struct sockaddr_in server_addr, client_addr;
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(server_sock, 10);

    while (1) {
        memset(&client_addr, 0, sizeof(client_addr));
        socklen_t client_size = sizeof(client_addr);
        int client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_size);

        if (g_num_clients >= MAX_CLIENTS) {
            send(client_sock, "Server is full.\r\n", 16, 0);
            close(client_sock);
            continue;
        }

        g_client_socks[g_num_clients] = client_sock;
        g_num_clients++;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client_sock);
    }

    close(server_sock);
}

int main() {
    accept_connections();

    return 0;
}