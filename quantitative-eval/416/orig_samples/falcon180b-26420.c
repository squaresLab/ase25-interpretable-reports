//Falcon-180B DATASET v1.0 Category: Smart home light control ; Style: paranoid
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int sockfd;
int client_socks[MAX_CLIENTS];
int client_count;

struct sockaddr_in server_addr;
struct sockaddr_in client_addr;

char buffer[BUFFER_SIZE];

void *handle_client(void *arg) {
    int sock = *(int *) arg;
    char *response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Smart Home Light Control</h1></body></html>";
    int len = strlen(response);

    send(sock, response, len, 0);
    close(sock);
}

void accept_connections() {
    while(1) {
        socklen_t addr_size = sizeof(client_addr);
        int new_socket = accept(sockfd, (struct sockaddr *) &client_addr, &addr_size);

        if(new_socket == -1) {
            perror("accept");
            continue;
        }

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &new_socket);
    }
}

int main() {
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(sockfd, MAX_CLIENTS);

    accept_connections();

    return 0;
}