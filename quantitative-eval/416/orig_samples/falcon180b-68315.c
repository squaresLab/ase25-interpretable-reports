//Falcon-180B DATASET v1.0 Category: Chat server ; Style: surrealist
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <pthread.h>

#define PORT 8080
#define BACKLOG 10
#define MAX_CLIENTS 10
#define MAX_MESSAGE_SIZE 1024

int clients[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[MAX_MESSAGE_SIZE];

    while(1) {
        memset(buffer, 0, MAX_MESSAGE_SIZE);
        int bytes_received = recv(client_socket, buffer, MAX_MESSAGE_SIZE, 0);

        if(bytes_received <= 0) {
            break;
        }

        for(int i=0; i<num_clients; i++) {
            if(clients[i]!= client_socket) {
                send(clients[i], buffer, strlen(buffer), 0);
            }
        }
    }

    close(client_socket);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, BACKLOG);

    printf("Chat Server started on port %d\n", PORT);

    while(1) {
        int client_socket = accept(server_socket, NULL, NULL);

        if(num_clients >= MAX_CLIENTS) {
            send(client_socket, "Server is full. Please try again later.\n", 46, 0);
            close(client_socket);
            continue;
        }

        clients[num_clients++] = client_socket;
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket);
    }

    return 0;
}