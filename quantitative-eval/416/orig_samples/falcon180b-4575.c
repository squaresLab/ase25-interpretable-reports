//Falcon-180B DATASET v1.0 Category: Client Server Application ; Style: curious
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BACKLOG 10
#define MAX_REQUEST_SIZE 1024
#define MAX_CLIENTS 10

int listen_socket;
int client_sockets[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char request[MAX_REQUEST_SIZE];
    char response[MAX_REQUEST_SIZE];
    while (1) {
        memset(request, 0, MAX_REQUEST_SIZE);
        int bytes_received = recv(client_socket, request, MAX_REQUEST_SIZE, 0);
        if (bytes_received <= 0) {
            break;
        }
        printf("Received request: %s\n", request);
        strcat(response, "HTTP/1.1 200 OK\r\n");
        strcat(response, "Content-Type: text/html\r\n");
        strcat(response, "Connection: close\r\n");
        strcat(response, "\r\n");
        strcat(response, "<html><body><h1>Hello, World!</h1></body></html>");
        send(client_socket, response, strlen(response), 0);
    }
    close(client_socket);
    num_clients--;
    printf("Client disconnected\n");
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket, BACKLOG) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);
    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        if (num_clients >= MAX_CLIENTS) {
            send(client_socket, "HTTP/1.1 503 Service Unavailable\r\n", strlen("HTTP/1.1 503 Service Unavailable\r\n"), 0);
            close(client_socket);
            continue;
        }
        client_sockets[num_clients] = client_socket;
        num_clients++;
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
    }
    return 0;
}