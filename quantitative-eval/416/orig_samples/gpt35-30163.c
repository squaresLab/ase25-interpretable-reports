//FormAI DATASET v1.0 Category: Client Server Application ; Style: ephemeral
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 5555

void *connection_handler(void *);

int main(int argc, char *argv[]) {

    int socket_desc, new_socket, c;
    struct sockaddr_in server, client;

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed. Error");
        return 1;
    }

    listen(socket_desc, 3);

    printf("Waiting for incoming connections...\n");
    c = sizeof(struct sockaddr_in);
    while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))) {

        printf("Connection accepted from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

        pthread_t sniffer_thread;
        if (pthread_create(&sniffer_thread, NULL, connection_handler, (void*) &new_socket) < 0) {
            perror("Could not create thread");
            return 1;
        }

        printf("Handler assigned\n");
    }

    if (new_socket < 0) {
        perror("Accept failed");
        return 1;
    }

    return 0;
}

void *connection_handler(void *socket_desc) {

    int sock = *(int*)socket_desc;
    char message_buffer[2000];
    char *server_message = "Hello! Welcome to my chat server\n";

    if (send(sock, server_message, strlen(server_message), 0) != strlen(server_message)) {
        perror("Send failed");
    }

    while (recv(sock, message_buffer, 2000, 0) > 0) {

        printf("Client: %s\n", message_buffer);
        memset(message_buffer, 0, 2000);
        printf("Server: ");
        fgets(message_buffer, 2000, stdin);
        message_buffer[strlen(message_buffer)-1] = '\0';

        if (send(sock, message_buffer, strlen(message_buffer), 0) < 0) {
            perror("Send failed");
        }

        memset(message_buffer, 0, 2000);
    }

    if (recv(sock, message_buffer, 2000, 0) < 0) {
        puts("Receive failed");
    }

    close(sock);
    pthread_exit(NULL);
}