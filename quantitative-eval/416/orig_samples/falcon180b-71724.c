//Falcon-180B DATASET v1.0 Category: Smart home automation ; Style: surrealist
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define TRUE 1
#define FALSE 0
#define NUM_THREADS 5

int client_sockets[MAX_CLIENTS];
int num_clients = 0;

sem_t mutex;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[BUFFER_SIZE];

    while (1) {
        sem_wait(&mutex);
        if (num_clients == 0) {
            sem_post(&mutex);
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            printf("Client disconnected\n");
            close(client_socket);
            num_clients--;
            sem_post(&mutex);
            pthread_exit(NULL);
        }

        printf("Received message from client: %s\n", buffer);
        send(client_socket, buffer, strlen(buffer), 0);
    }
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr;

    sem_init(&mutex, 0, 1);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));

    listen(server_socket, 10);

    printf("Server is listening on port 8080\n");

    while (1) {
        int client_socket;
        socklen_t client_len;

        client_socket = accept(server_socket, (struct sockaddr *) &server_addr, &client_len);

        if (num_clients >= MAX_CLIENTS) {
            close(client_socket);
            printf("Server is full\n");
        } else {
            client_sockets[num_clients] = client_socket;
            num_clients++;

            printf("New client connected\n");

            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
        }
    }

    return 0;
}