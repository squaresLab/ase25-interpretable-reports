//Falcon-180B DATASET v1.0 Category: Expense Tracker ; Style: multiplayer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

char *clients[MAX_CLIENTS];
int num_clients = 0;

void *listen_thread(void *arg) {
    int sock = *(int *)arg;
    char buffer[BUFFER_SIZE];
    int new_socket;

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        printf("Received message: %s\n", buffer);

        for (int i = 0; i < num_clients; i++) {
            send(clients[i], buffer, strlen(buffer), 0);
        }
    }

    close(sock);
    pthread_exit(NULL);
}

void *client_thread(void *arg) {
    int sock = *(int *)arg;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        printf("Received message: %s\n", buffer);
    }

    close(sock);
    pthread_exit(NULL);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(server_socket, 10);

    printf("Server started on port %d\n", PORT);

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);

        if (num_clients >= MAX_CLIENTS) {
            send(client_socket, "Server is full. Please try again later.\n", 46, 0);
            close(client_socket);
            continue;
        }

        clients[num_clients] = (char *)malloc(sizeof(char) * 20);
        sprintf(clients[num_clients], "Client %d", num_clients + 1);

        num_clients++;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, client_thread, (void *)&client_socket);
    }

    return 0;
}