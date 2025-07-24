//FormAI DATASET v1.0 Category: TCP/IP Programming ; Style: curious
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8888
#define MAX_CLIENTS 10

int clients[MAX_CLIENTS];
int num_clients = 0;
pthread_mutex_t mutex;

void *client_handler(void *arg) {
    int sock_fd = *(int *)arg;
    char buffer[1024];
    while (1) {
        int len = read(sock_fd, buffer, 1024);
        if (len <= 0) break;
        buffer[len] = '\0';
        printf("Client %d: %s", sock_fd, buffer);
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < num_clients; i++) {
            if (clients[i] != sock_fd) {
                write(clients[i], buffer, strlen(buffer));
            }
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < num_clients; i++) {
        if (clients[i] == sock_fd) {
            for (int j = i+1; j < num_clients; j++) {
                clients[j-1] = clients[j];
            }
            num_clients--;
            break;
        }
    }
    close(sock_fd);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    int server_fd;
    struct sockaddr_in addr;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server started listening on port %d!\n", PORT);

    while (1) {
        int client_fd;
        pthread_t thread;

        if ((client_fd = accept(server_fd, NULL, NULL)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        pthread_mutex_lock(&mutex);
        if (num_clients >= MAX_CLIENTS) {
            printf("Client %d denied: maximum clients reached!\n", client_fd);
            close(client_fd);
        } else {
            printf("Client %d connected!\n", client_fd);
            clients[num_clients++] = client_fd;
            pthread_create(&thread, NULL, client_handler, (void *)&client_fd);
            pthread_detach(thread);
        }
        pthread_mutex_unlock(&mutex);
    }

    close(server_fd);
    return 0;
}