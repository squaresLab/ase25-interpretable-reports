//Falcon-180B DATASET v1.0 Category: Intrusion detection system ; Style: active
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int client_sockets[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[BUFFER_SIZE];
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            break;
        }
        if (strstr(buffer, "attack")!= NULL) {
            printf("Attack detected from client %d\n", client_socket);
        }
    }
    close(client_socket);
    return NULL;
}

void accept_connections() {
    while (num_clients < MAX_CLIENTS) {
        int server_socket = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(8080);
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
        listen(server_socket, 5);

        while (1) {
            int client_socket = accept(server_socket, NULL, NULL);
            if (client_socket == -1) {
                printf("Error accepting connection\n");
                break;
            }
            printf("Client %d connected\n", num_clients);
            client_sockets[num_clients] = client_socket;
            num_clients++;
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket);
        }

        close(server_socket);
    }
}

int main() {
    accept_connections();
    return 0;
}