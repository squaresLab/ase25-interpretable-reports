//Falcon-180B DATASET v1.0 Category: Client Server Application ; Style: surrealist
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BACKLOG 10
#define MAX_REQUEST_SIZE 1024
#define MAX_RESPONSE_SIZE 4096

int server_socket;
int client_socket;

pthread_t thread_id;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

char request_buffer[MAX_REQUEST_SIZE];
char response_buffer[MAX_RESPONSE_SIZE];

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;

    pthread_mutex_lock(&lock);
    printf("Client %d connected\n", client_socket);
    pthread_mutex_unlock(&lock);

    while (1) {
        memset(request_buffer, 0, MAX_REQUEST_SIZE);
        int bytes_received = recv(client_socket, request_buffer, MAX_REQUEST_SIZE, 0);

        if (bytes_received <= 0) {
            close(client_socket);
            pthread_mutex_lock(&lock);
            printf("Client %d disconnected\n", client_socket);
            pthread_mutex_unlock(&lock);
            break;
        }

        pthread_mutex_lock(&lock);
        printf("Request received from client %d: %s\n", client_socket, request_buffer);
        pthread_mutex_unlock(&lock);

        strcat(response_buffer, "Server response to client ");
        strcat(response_buffer, request_buffer);

        send(client_socket, response_buffer, strlen(response_buffer), 0);
    }

    return NULL;
}

int main() {
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);

        pthread_mutex_lock(&lock);
        printf("Client %d connected\n", client_socket);
        pthread_mutex_unlock(&lock);

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
    }

    close(server_socket);
    return 0;
}