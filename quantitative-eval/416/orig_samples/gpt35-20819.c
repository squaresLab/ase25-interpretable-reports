//FormAI DATASET v1.0 Category: Network Quality of Service (QoS) monitor ; Style: modular
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

#define MAX_CLIENTS 100
#define MAX_DATA_SIZE 4096
#define QOS_THRESHOLD 1000

int client_count = 0;
int client_sockets[MAX_CLIENTS];

void handle_signal(int sig) {
    printf("Closing connections...\n");
    for(int i = 0; i < client_count; i++) {
        close(client_sockets[i]);
    }
    exit(0);
}

void* handle_client(void* socket_ptr) {
    int socket = *(int*)socket_ptr;
    char data[MAX_DATA_SIZE];
    while(1) {
        int bytes_received = recv(socket, data, sizeof(data), 0);
        if(bytes_received <= 0) {
            break;
        }
        // Do some processing here...
        printf("Received %d bytes from client\n", bytes_received);
        if(bytes_received > QOS_THRESHOLD) {
            printf("Quality of Service threshold exceeded!\n");
            // Do something to handle this...
        }
    }
    close(socket);
}

int main() {
    signal(SIGINT, handle_signal);
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0) {
        perror("socket() failed");
        exit(1);
    }
    struct sockaddr_in server_address = {0};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8000);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        perror("bind() failed");
        exit(1);
    }
    if(listen(server_socket, MAX_CLIENTS) < 0) {
        perror("listen() failed");
        exit(1);
    }
    printf("Server running...\n");
    while(1) {
        struct sockaddr_in client_address = {0};
        socklen_t client_address_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_address_len);
        if(client_socket < 0) {
            perror("accept() failed");
            exit(1);
        }
        client_sockets[client_count++] = client_socket;
        printf("New client connected: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        pthread_t thread;
        if(pthread_create(&thread, NULL, handle_client, &client_socket) < 0) {
            perror("pthread_create() failed");
            exit(1);
        }
        pthread_detach(thread);
    }
    return 0;
}