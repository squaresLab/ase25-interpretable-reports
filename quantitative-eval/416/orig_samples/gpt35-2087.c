//FormAI DATASET v1.0 Category: Socket programming ; Style: dynamic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUF_SIZE 1024

int client_count = 0;
int clients[MAX_CLIENTS];

void *handle_client(void *arg) {
    int clientfd = *(int *)arg;
    char buffer[BUF_SIZE];
    char welcome[BUF_SIZE] = "Welcome to the chat room!\n";

    // Send welcome message to new client
    if (send(clientfd, welcome, strlen(welcome), 0) == -1) {
        perror("Failed to send welcome message");
        return NULL;
    }

    // Add client to list of active clients
    clients[client_count++] = clientfd;

    // Start chat loop
    while (1) {
        int nbytes_recv = recv(clientfd, buffer, BUF_SIZE, 0);
        if (nbytes_recv <= 0) {
            // Handle disconnected client
            for (int i = 0; i < client_count; i++) {
                if (clients[i] == clientfd) {
                    close(clients[i]);
                    clients[i] = -1;
                    break;
                }
            }
            break;
        } else {
            // Forward message to all clients
            for (int i = 0; i < client_count; i++) {
                if (clients[i] != -1 && clients[i] != clientfd) {
                    if (send(clients[i], buffer, nbytes_recv, 0) == -1) {
                        perror("Failed to forward message");
                    }
                }
            }
        }
    }

    // Remove client from list of active clients
    for (int i = 0; i < client_count; i++) {
        if (clients[i] == clientfd) {
            clients[i] = -1;
            break;
        }
    }

    close(clientfd);
    return NULL;
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9001);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, MAX_CLIENTS) == -1) {
        perror("Failed to listen");
        exit(EXIT_FAILURE);
    }

    printf("Server started on port 9001...\n");

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        int clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &len);

        if (clientfd == -1) {
            perror("Failed to accept connection");
            continue;
        }

        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, &clientfd) != 0) {
            perror("Failed to create thread");
            continue;
        }

        // Detach thread to avoid memory leaks
        if (pthread_detach(tid) != 0) {
            perror("Failed to detach thread");
            continue;
        }
    }

    return 0;
}