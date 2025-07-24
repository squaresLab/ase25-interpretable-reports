//Falcon-180B DATASET v1.0 Category: Intrusion detection system ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10
#define MAX_REQUEST_SIZE 2048

int clients[MAX_CLIENTS];
int num_clients = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    char request[MAX_REQUEST_SIZE];
    int bytes_received = 0;

    while ((bytes_received = recv(client_socket, request, MAX_REQUEST_SIZE, 0)) > 0) {
        char *token;
        char *method = strtok_r(request, " ", &token);
        char *path = strtok_r(NULL, " ", &token);

        if (strcasecmp(method, "GET") == 0 && strcasecmp(path, "/") == 0) {
            // Send response for GET / request
            send(client_socket, "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!", 50, 0);
        } else {
            // Send response for all other requests
            send(client_socket, "HTTP/1.1 404 Not Found\r\nContent-Length: 16\r\n\r\n404 Not Found", 46, 0);
        }
    }

    close(client_socket);
    return NULL;
}

void accept_connections(int server_socket) {
    while (num_clients < MAX_CLIENTS) {
        int client_socket = accept(server_socket, NULL, NULL);

        if (client_socket >= 0) {
            pthread_mutex_lock(&lock);
            clients[num_clients] = client_socket;
            num_clients++;
            pthread_mutex_unlock(&lock);

            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, &client_socket);
        }
    }
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0) {
        printf("Error creating socket\n");
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error binding socket\n");
        return 1;
    }

    if (listen(server_socket, 10) < 0) {
        printf("Error listening on socket\n");
        return 1;
    }

    printf("Server started on port 8080\n");
    accept_connections(server_socket);

    return 0;
}