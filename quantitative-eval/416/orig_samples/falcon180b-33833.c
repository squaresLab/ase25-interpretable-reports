//Falcon-180B DATASET v1.0 Category: Simple HTTP Proxy ; Style: multi-threaded
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define MAX_REQUEST_SIZE 4096
#define HTTP_PORT 8080

int client_sockets[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char request[MAX_REQUEST_SIZE];
    char response[MAX_REQUEST_SIZE];
    int bytes_received = 0;
    int response_code = 200;

    while (1) {
        memset(request, 0, MAX_REQUEST_SIZE);
        bytes_received = recv(client_socket, request, MAX_REQUEST_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        response[0] = '\0';
        strcat(response, "HTTP/1.1 ");
        sprintf(response + strlen(response), "%d ", response_code);
        strcat(response, "OK");
        strcat(response, "\r\n");
        strcat(response, "Content-Type: text/html\r\n");
        strcat(response, "Connection: close\r\n");
        strcat(response, "\r\n");
        strcat(response, "<html><body><h1>Hello, World!</h1></body></html>");

        send(client_socket, response, strlen(response), 0);
    }

    close(client_socket);
    return NULL;
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;

    if (server_socket == -1) {
        printf("Error creating socket\n");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(HTTP_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        printf("Error binding socket\n");
        return 1;
    }

    if (listen(server_socket, MAX_CLIENTS) == -1) {
        printf("Error listening on socket\n");
        return 1;
    }

    printf("Server started on port %d\n", HTTP_PORT);

    while (1) {
        socklen_t addr_size = sizeof(struct sockaddr_in);
        int client_socket = accept(server_socket, (struct sockaddr *) &server_addr, &addr_size);

        if (client_socket == -1) {
            printf("Error accepting connection\n");
            continue;
        }

        if (num_clients >= MAX_CLIENTS) {
            printf("Maximum number of clients reached\n");
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