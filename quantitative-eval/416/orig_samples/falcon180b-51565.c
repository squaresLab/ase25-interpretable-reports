//Falcon-180B DATASET v1.0 Category: Smart home light control ; Style: synchronous
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define PORT 8080
#define BUFFER_SIZE 1024

int client_sockets[MAX_CLIENTS];
int client_socket_count = 0;

void *handle_client(void *socket_desc) {
    int sock = *(int *) socket_desc;
    char buffer[BUFFER_SIZE];

    while(1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);

        if(bytes_received <= 0) {
            break;
        }

        printf("Received message: %s\n", buffer);

        for(int i=0; i<client_socket_count; i++) {
            if(client_sockets[i]!= sock) {
                send(client_sockets[i], buffer, strlen(buffer), 0);
            }
        }
    }

    close(sock);
    return NULL;
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);

    printf("Server started on port %d\n", PORT);

    while(1) {
        int client_socket = accept(server_socket, NULL, NULL);

        if(client_socket_count >= MAX_CLIENTS) {
            printf("Maximum clients reached\n");
            close(client_socket);
            continue;
        }

        client_sockets[client_socket_count] = client_socket;
        client_socket_count++;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket);
    }

    return 0;
}