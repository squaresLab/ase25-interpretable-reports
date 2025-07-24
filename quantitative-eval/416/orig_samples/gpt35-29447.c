//FormAI DATASET v1.0 Category: Networking ; Style: multi-threaded
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

void *connection_handler(void *);

int main(int argc, char *argv[]) {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 3) < 0) {
        perror("listen error");
        exit(EXIT_FAILURE);
    }

    puts("waiting for connections...");

    while(client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) {
        puts("connection accepted");

        pthread_t thread_id;

        if(pthread_create(&thread_id, NULL, connection_handler, (void *)&client_fd) < 0) {
            perror("could not create thread");
            exit(EXIT_FAILURE);
        }
    }

    if(client_fd < 0) {
        perror("accept error");
        exit(EXIT_FAILURE);
    }

    return 0;
}

void *connection_handler(void *args) {
    int client_fd = *(int *)args;
    char buffer[1024] = { 0 };
    char *welcome_message = "Welcome to my server!\n";
    int valread;

    send(client_fd, welcome_message, strlen(welcome_message), 0);

    while((valread = read(client_fd, buffer, 1024)) > 0) {
        send(client_fd, buffer, strlen(buffer), 0);
        memset(buffer, 0, sizeof(buffer));
    }

    puts("client disconnected");

    return 0;
}