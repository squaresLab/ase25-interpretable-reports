//GEMINI-pro DATASET v1.0 Category: Simple HTTP Proxy ; Style: unmistakable
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENTS 10

typedef struct {
    int sockfd;
    struct sockaddr_in addr;
} client_t;

typedef struct {
    int num_clients;
    client_t clients[MAX_CLIENTS];
} proxy_t;

proxy_t proxy;

void *handle_client(void *arg) {
    client_t *client = (client_t *)arg;
    char buffer[1024];
    int n;

    while ((n = read(client->sockfd, buffer, sizeof(buffer))) > 0) {
        // Parse the HTTP request
        char *method = strtok(buffer, " ");
        char *path = strtok(NULL, " ");
        char *version = strtok(NULL, "\r\n");

        // Check if the request is valid
        if (strcmp(method, "GET") != 0 || strcmp(version, "HTTP/1.1") != 0) {
            write(client->sockfd, "HTTP/1.1 400 Bad Request\r\n\r\n", 32);
            close(client->sockfd);
            return NULL;
        }

        // Connect to the remote server
        int remote_sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (remote_sockfd == -1) {
            write(client->sockfd, "HTTP/1.1 500 Internal Server Error\r\n\r\n", 39);
            close(client->sockfd);
            return NULL;
        }

        struct sockaddr_in remote_addr;
        remote_addr.sin_family = AF_INET;
        remote_addr.sin_port = htons(80);
        remote_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

        if (connect(remote_sockfd, (struct sockaddr *)&remote_addr, sizeof(remote_addr)) == -1) {
            write(client->sockfd, "HTTP/1.1 500 Internal Server Error\r\n\r\n", 39);
            close(client->sockfd);
            return NULL;
        }

        // Forward the request to the remote server
        write(remote_sockfd, buffer, n);

        // Read the response from the remote server
        while ((n = read(remote_sockfd, buffer, sizeof(buffer))) > 0) {
            // Forward the response to the client
            write(client->sockfd, buffer, n);
        }

        // Close the connections
        close(client->sockfd);
        close(remote_sockfd);
    }

    return NULL;
}

int main(int argc, char **argv) {
    proxy.num_clients = 0;

    int listen_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sockfd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in listen_addr;
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = htons(8080);
    listen_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_sockfd, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) == -1) {
        perror("bind");
        return -1;
    }

    if (listen(listen_sockfd, MAX_CLIENTS) == -1) {
        perror("listen");
        return -1;
    }

    while (1) {
        client_t client;
        socklen_t len = sizeof(client.addr);

        client.sockfd = accept(listen_sockfd, (struct sockaddr *)&client.addr, &len);
        if (client.sockfd == -1) {
            perror("accept");
            continue;
        }

        if (proxy.num_clients >= MAX_CLIENTS) {
            write(client.sockfd, "HTTP/1.1 503 Service Unavailable\r\n\r\n", 36);
            close(client.sockfd);
            continue;
        }

        proxy.clients[proxy.num_clients++] = client;

        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, &client);
    }

    return 0;
}