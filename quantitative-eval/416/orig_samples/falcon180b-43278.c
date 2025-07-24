//Falcon-180B DATASET v1.0 Category: Network Quality of Service (QoS) monitor ; Style: shocked
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>

#define MAX_THREADS 10
#define MAX_CONNECTIONS 100
#define MAX_REQUEST_SIZE 1024
#define MAX_RESPONSE_SIZE 4096

struct connection {
    int socket;
    char ip[INET6_ADDRSTRLEN];
    int port;
};

struct thread_data {
    int thread_id;
    struct connection *connections;
};

void *monitor_thread(void *arg) {
    struct thread_data *data = (struct thread_data *) arg;
    struct connection *connections = data->connections;

    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (connections[i].socket!= -1) {
            char request[MAX_REQUEST_SIZE];
            memset(request, 0, MAX_REQUEST_SIZE);

            int bytes_received = recv(connections[i].socket, request, MAX_REQUEST_SIZE, 0);

            if (bytes_received <= 0) {
                close(connections[i].socket);
                connections[i].socket = -1;
                continue;
            }

            char response[MAX_RESPONSE_SIZE];
            memset(response, 0, MAX_RESPONSE_SIZE);

            sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n", bytes_received);

            send(connections[i].socket, response, strlen(response), 0);
        }
    }
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1) {
        printf("Error creating socket\n");
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        printf("Error binding socket\n");
        return 1;
    }

    if (listen(server_socket, MAX_CONNECTIONS) == -1) {
        printf("Error listening on socket\n");
        return 1;
    }

    printf("Server started on port 8080\n");

    struct connection connections[MAX_CONNECTIONS];
    memset(connections, 0, sizeof(connections));

    pthread_t threads[MAX_THREADS];

    for (int i = 0; i < MAX_THREADS; i++) {
        struct thread_data data;
        data.thread_id = i;
        data.connections = connections;

        pthread_create(&threads[i], NULL, monitor_thread, (void *) &data);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    close(server_socket);

    return 0;
}