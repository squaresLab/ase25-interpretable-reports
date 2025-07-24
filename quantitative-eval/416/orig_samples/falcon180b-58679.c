//Falcon-180B DATASET v1.0 Category: Smart home light control ; Style: creative
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define PORT 8080
#define BACKLOG 10

int sock;
struct sockaddr_in server_addr;
struct sockaddr_in client_addr[MAX_CLIENTS];
int client_sock[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *arg) {
    int client_sock = *(int *)arg;
    char buffer[1024];
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            break;
        }
        printf("Client %d said: %s\n", client_sock, buffer);
        send(client_sock, buffer, strlen(buffer), 0);
    }
    close(client_sock);
    num_clients--;
}

int main() {
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(sock, BACKLOG);

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        int client_sock = accept(sock, (struct sockaddr *)&client_addr[num_clients], sizeof(client_addr[num_clients]));
        if (client_sock == -1) {
            printf("Error accepting connection\n");
            continue;
        }
        printf("Client %d connected\n", client_sock);
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *)&client_sock);
        num_clients++;
    }

    return 0;
}