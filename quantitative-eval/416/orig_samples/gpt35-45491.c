//FormAI DATASET v1.0 Category: Socket programming ; Style: mind-bending
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 5

void *client_handler(void *arg);

int main() {

    // create socket file descriptor
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Failed to create socket descriptor\n");
        return -1;
    }

    // prepare sockaddr_in structure
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // bind socket to address and port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        printf("Failed to bind socket to address and port\n");
        close(server_socket);
        return -1;
    }

    // listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) < 0) {
        printf("Failed to listen for incoming connections\n");
        close(server_socket);
        return -1;
    }

    // accept incoming connections and handle them in separate threads
    while (1) {

        // accept incoming connection
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket < 0) {
            printf("Failed to accept incoming connection\n");
            close(server_socket);
            return -1;
        }

        // create thread for client handling
        pthread_t tid;
        if (pthread_create(&tid, NULL, client_handler, (void *)&client_socket) < 0) {
            printf("Failed to create client handling thread\n");
            close(client_socket);
            continue;
        }
    }
}

void *client_handler(void *arg) {

    // get socket file descriptor from argument
    int client_socket = *(int *)arg;

    // receive message from client
    char buffer[1024] = {0};
    if (recv(client_socket, buffer, 1024, 0) < 0) {
        printf("Failed to receive message from client\n");
        close(client_socket);
        pthread_exit(NULL);
    }

    // check if message is a palindrome
    int left = 0, right = strlen(buffer) - 1;
    while (right > left) {
        if (buffer[left++] != buffer[right--]) {
            // not a palindrome, send response to client
            char response[] = "Not a palindrome\n";
            send(client_socket, response, strlen(response), 0);
            close(client_socket);
            pthread_exit(NULL);
        }
    }

    // message is a palindrome, send response to client
    char response[] = "Palindrome\n";
    send(client_socket, response, strlen(response), 0);
    close(client_socket);
    pthread_exit(NULL);
}