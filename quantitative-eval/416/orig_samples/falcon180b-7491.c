//Falcon-180B DATASET v1.0 Category: Email Client ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX 1000
#define PORT 8080
#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int server_socket;
int client_socket;
struct sockaddr_in server_addr;
struct sockaddr_in client_addr;

char *message = "Welcome to the email server!\n";
char buffer[BUFFER_SIZE];
char username[20];
char password[20];

void *handle_client(void *args) {
    int client_socket = *((int *) args);
    printf("New client connected with socket %d\n", client_socket);

    while(1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if(bytes_received <= 0) {
            printf("Client %d disconnected\n", client_socket);
            close(client_socket);
            pthread_exit(NULL);
        }

        printf("Received message from client %d: %s", client_socket, buffer);
    }
}

int main() {
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    while(1) {
        printf("Waiting for client connection...\n");
        int client_socket = accept(server_socket, (struct sockaddr *) &client_addr, sizeof(client_addr));
        printf("Client connected with socket %d\n", client_socket);

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
    }

    return 0;
}