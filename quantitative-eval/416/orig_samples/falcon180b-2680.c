//Falcon-180B DATASET v1.0 Category: Simple HTTP Proxy ; Style: intelligent
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define MAX_LINE 4096
#define PORT 8080

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int create_socket(int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        error("socket failed");
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        error("bind failed");
    }

    if (listen(sockfd, 5) < 0) {
        error("listen failed");
    }

    return sockfd;
}

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[MAX_LINE];

    while (1) {
        memset(buffer, 0, MAX_LINE);
        int bytes_received = recv(client_socket, buffer, MAX_LINE, 0);
        if (bytes_received <= 0) {
            break;
        }

        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
    return NULL;
}

int main() {
    int server_socket = create_socket(PORT);

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket);
    }

    return 0;
}