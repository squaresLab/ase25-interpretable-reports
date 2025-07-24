//FormAI DATASET v1.0 Category: Simple Web Server ; Style: decentralized
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#define MAX_CONNECTIONS 10
#define PORT_NUMBER 8080
#define BUFFER_SIZE 1024

int server_socket;
pthread_t threads[MAX_CONNECTIONS];

void handle_request(int socket_fd) {
    char buffer[BUFFER_SIZE];
    read(socket_fd, buffer, BUFFER_SIZE);
    printf("Received request: %s\n", buffer);
    char response[BUFFER_SIZE] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello World!";
    write(socket_fd, response, strlen(response));
    close(socket_fd);
}

void *handle_connection(void *arg) {
    int socket_fd = *((int *) arg);
    handle_request(socket_fd);
    return NULL;
}

void signal_handler() {
    printf("Shutting down server...\n");
    close(server_socket);
    exit(0);
}

int main() {
    // Ignore SIGPIPE signal to prevent crashes when writing to closed sockets
    signal(SIGPIPE, SIG_IGN);
    
    // Set up server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = { 0 };
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT_NUMBER);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CONNECTIONS);
    
    // Set up handler for SIGINT signal to gracefully shutdown server
    signal(SIGINT, signal_handler);

    // Accept incoming connections and spawn threads to handle them
    while (1) {
        struct sockaddr_in client_addr = { 0 };
        socklen_t client_addr_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &client_addr_len);
        pthread_t thread;
        pthread_create(&thread, NULL, handle_connection, &client_socket);
        for (int i = 0; i < MAX_CONNECTIONS; i++) {
            if (!threads[i]) {
                threads[i] = thread;
                break;
            }
            if (i == MAX_CONNECTIONS - 1) {
                printf("Maximum number of connections reached.\n");
                close(client_socket);
            }
        }
        pthread_detach(thread);
    }
    return 0;
}