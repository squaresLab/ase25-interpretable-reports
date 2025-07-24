//Falcon-180B DATASET v1.0 Category: Banking Record System ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define MAX_REQUEST_SIZE 1024

int sockfd;
int clients[MAX_CLIENTS];
int num_clients;

void *handle_client(void *client_socket) {
    int client_sock = *(int *) client_socket;
    char request[MAX_REQUEST_SIZE];
    bzero(request, MAX_REQUEST_SIZE);

    while (1) {
        memset(request, 0, MAX_REQUEST_SIZE);
        int bytes_received = recv(client_sock, request, MAX_REQUEST_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        printf("Request from client %d: %s\n", client_sock, request);

        char response[MAX_REQUEST_SIZE];
        sprintf(response, "Response to client %d", client_sock);

        send(client_sock, response, strlen(response), 0);
    }

    close(client_sock);
    return NULL;
}

int main() {
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(sockfd, MAX_CLIENTS);

    printf("Server started on port %d\n", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_size = sizeof(client_addr);
        int client_sock = accept(sockfd, (struct sockaddr *) &client_addr, &addr_size);

        if (num_clients == MAX_CLIENTS) {
            printf("Maximum clients reached\n");
            close(client_sock);
        } else {
            clients[num_clients] = client_sock;
            num_clients++;

            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, (void *) &client_sock);
        }
    }

    return 0;
}