//Falcon-180B DATASET v1.0 Category: Network Quality of Service (QoS) monitor ; Style: ephemeral
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_SOCKETS 10
#define BUFFER_SIZE 1024

int sockets[MAX_SOCKETS];
int num_sockets = 0;

void *monitor_thread(void *arg) {
    int sock = *(int *) arg;
    char buffer[BUFFER_SIZE];
    int bytes_received;

    while (1) {
        bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            break;
        }

        printf("Received %d bytes on socket %d\n", bytes_received, sock);
    }

    close(sock);
    return NULL;
}

void start_monitoring(int sock) {
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, monitor_thread, (void *) &sock);
    num_sockets++;
}

int main() {
    int server_sock, client_sock, new_sock;
    struct sockaddr_in server_addr, client_addr;
    int opt = 1;
    int addrlen = sizeof(client_addr);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(12345);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        printf("Error creating socket\n");
        exit(1);
    }

    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        printf("Error setting socket option\n");
        exit(1);
    }

    if (bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        printf("Error binding socket\n");
        exit(1);
    }

    if (listen(server_sock, MAX_SOCKETS) == -1) {
        printf("Error listening on socket\n");
        exit(1);
    }

    while (1) {
        client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &addrlen);
        if (client_sock == -1) {
            printf("Error accepting connection\n");
            continue;
        }

        printf("Accepted connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        start_monitoring(client_sock);
    }

    return 0;
}