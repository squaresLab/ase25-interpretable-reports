//FormAI DATASET v1.0 Category: Client Server Application ; Style: multi-threaded
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX_CLIENTS 10

int client_count = 0;
int client_sockets[MAX_CLIENTS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *client_handler(void *arg) {
    int client_socket = *(int *)arg;
    char buffer[1024];
    char message[1024];
    int n;

    pthread_mutex_lock(&mutex);
    client_count++;
    pthread_mutex_unlock(&mutex);

    printf("New client connected - ID %d\n", client_socket);

    while ((n = read(client_socket, buffer, 1024)) > 0) {
        buffer[n] = '\0';
        sprintf(message, "Client %d: %s", client_socket, buffer);
        printf("%s", message);

        pthread_mutex_lock(&mutex);
        for (int i = 0; i < client_count; i++) {
            if (client_sockets[i] != client_socket) {
                write(client_sockets[i], message, strlen(message));
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex);
    for (int i = 0; i < client_count; i++) {
        if (client_sockets[i] == client_socket) {
            for (int j = i; j < client_count-1; j++) {
                client_sockets[j] = client_sockets[j+1];
            }
            client_count--;
            break;
        }
    }
    close(client_socket);
    pthread_mutex_unlock(&mutex);

    printf("Client %d disconnected\n", client_socket);

    return NULL;
}

int main() {
    int server_socket, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t tid;

    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind error");
        return -1;
    }

    if (listen(server_socket, 3) < 0) {
        perror("Listen error");
        return -1;
    }

    printf("Server started and listening on port %d\n", PORT);

    while (1) {
        if ((new_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept error");
            break;
        }

        pthread_create(&tid, NULL, &client_handler, &new_socket);

        pthread_mutex_lock(&mutex);
        client_sockets[client_count] = new_socket;
        pthread_mutex_unlock(&mutex);
    }

    close(server_socket);
    return 0;
}