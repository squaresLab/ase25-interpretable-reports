//Falcon-180B DATASET v1.0 Category: Client Server Application ; Style: creative
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int g_socket;
struct sockaddr_in g_server_addr;
int g_num_clients = 0;
pthread_t g_thread_id[MAX_CLIENTS];

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[BUFFER_SIZE];
    while (TRUE) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            close(client_socket);
            g_num_clients--;
            pthread_exit(0);
        }
        printf("Client %d sent message: %s\n", client_socket, buffer);
        for (int i = 0; i < g_num_clients; i++) {
            if (g_thread_id[i]!= 0 && g_thread_id[i]!= pthread_self()) {
                send(g_thread_id[i], buffer, strlen(buffer), 0);
            }
        }
    }
}

int main() {
    int opt = TRUE;
    setsockopt(g_socket, SOL_SOCKET, SO_REUSEADDR, (char *) &opt, sizeof(opt));

    g_server_addr.sin_family = AF_INET;
    g_server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    g_server_addr.sin_port = htons(8080);

    bind(g_socket, (struct sockaddr *) &g_server_addr, sizeof(g_server_addr));
    listen(g_socket, MAX_CLIENTS);

    while (TRUE) {
        socklen_t addr_size = sizeof(g_server_addr);
        int client_socket = accept(g_socket, (struct sockaddr *) &g_server_addr, &addr_size);
        if (client_socket == -1) {
            perror("accept");
            continue;
        }
        printf("Client %d connected\n", client_socket);
        g_num_clients++;
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
    }

    close(g_socket);
    return 0;
}