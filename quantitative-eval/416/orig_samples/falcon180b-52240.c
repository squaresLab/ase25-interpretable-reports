//Falcon-180B DATASET v1.0 Category: Smart home light control ; Style: asynchronous
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BACKLOG 10
#define MAX_REQUEST_SIZE 1024

int g_sockfd;
struct sockaddr_in g_servaddr;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg) {
    int client_sockfd = *(int *)arg;
    char buf[MAX_REQUEST_SIZE];

    while (1) {
        memset(buf, 0, sizeof(buf));
        if (recv(client_sockfd, buf, MAX_REQUEST_SIZE, 0) <= 0) {
            break;
        }

        pthread_mutex_lock(&g_mutex);
        printf("Client %d: %s\n", client_sockfd, buf);
        pthread_mutex_unlock(&g_mutex);
    }

    close(client_sockfd);
    return NULL;
}

int main() {
    pthread_t tid;
    int opt = 1;
    int addrlen = sizeof(g_servaddr);

    g_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (g_sockfd == -1) {
        printf("socket failed\n");
        exit(1);
    }

    setsockopt(g_sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    g_servaddr.sin_family = AF_INET;
    g_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    g_servaddr.sin_port = htons(PORT);

    if (bind(g_sockfd, (struct sockaddr *)&g_servaddr, addrlen) == -1) {
        printf("bind failed\n");
        exit(1);
    }

    if (listen(g_sockfd, BACKLOG) == -1) {
        printf("listen failed\n");
        exit(1);
    }

    while (1) {
        int client_sockfd = accept(g_sockfd, (struct sockaddr *)&g_servaddr, &addrlen);
        if (client_sockfd == -1) {
            printf("accept failed\n");
            exit(1);
        }

        printf("Client connected: %d\n", client_sockfd);

        pthread_create(&tid, NULL, handle_client, (void *)&client_sockfd);
    }

    return 0;
}