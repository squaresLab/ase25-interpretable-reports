//Falcon-180B DATASET v1.0 Category: TCP/IP Programming ; Style: beginner-friendly
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/time.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define MAX_MESSAGE_LENGTH 1024

int client_sockets[MAX_CLIENTS];
int num_clients = 0;

void *listen_thread(void *arg) {
    int listen_socket = *(int *)arg;
    int new_socket;
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(client_addr);

    while (1) {
        new_socket = accept(listen_socket, (struct sockaddr *)&client_addr, &addr_size);

        if (new_socket == -1) {
            printf("Error accepting connection: %s\n", strerror(errno));
            continue;
        }

        if (num_clients >= MAX_CLIENTS) {
            close(new_socket);
            printf("Too many clients\n");
            continue;
        }

        client_sockets[num_clients++] = new_socket;
        printf("Client connected on socket %d\n", new_socket);
    }

    return NULL;
}

void broadcast_message(int sender_socket, char *message) {
    for (int i = 0; i < num_clients; i++) {
        if (i == sender_socket)
            continue;

        send(client_sockets[i], message, strlen(message), 0);
    }
}

int main() {
    int listen_socket;
    int opt = 1;
    struct sockaddr_in server_addr;

    if ((listen_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        printf("Error creating socket: %s\n", strerror(errno));
        exit(1);
    }

    setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3000);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listen_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("Error binding socket: %s\n", strerror(errno));
        exit(1);
    }

    if (listen(listen_socket, 10) == -1) {
        printf("Error listening on socket: %s\n", strerror(errno));
        exit(1);
    }

    printf("Server listening on port 3000\n");

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, listen_thread, (void *)&listen_socket);

    while (1) {
        char buffer[MAX_MESSAGE_LENGTH];
        int bytes_received;

        if ((bytes_received = recv(STDIN_FILENO, buffer, MAX_MESSAGE_LENGTH, 0)) == -1) {
            printf("Error receiving message: %s\n", strerror(errno));
            continue;
        }

        if (bytes_received == 0) {
            printf("Connection closed\n");
            break;
        }

        buffer[bytes_received - 1] = '\0';
        broadcast_message(STDIN_FILENO, buffer);
    }

    close(listen_socket);
    return 0;
}