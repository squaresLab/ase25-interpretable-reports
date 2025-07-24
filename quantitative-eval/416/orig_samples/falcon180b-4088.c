//Falcon-180B DATASET v1.0 Category: Smart home light control ; Style: sophisticated
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int clients[MAX_CLIENTS];
int num_clients = 0;
char buffer[BUFFER_SIZE];

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    char response[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (strcmp(buffer, "ON") == 0) {
            strcpy(response, "Lights turned on.");
        } else if (strcmp(buffer, "OFF") == 0) {
            strcpy(response, "Lights turned off.");
        } else {
            strcpy(response, "Invalid command.");
        }

        send(client_socket, response, strlen(response), 0);
    }

    close(client_socket);
    return NULL;
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;

    if (server_socket == -1) {
        printf("Error creating socket\n");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        printf("Error binding socket\n");
        exit(1);
    }

    if (listen(server_socket, MAX_CLIENTS) == -1) {
        printf("Error listening on socket\n");
        exit(1);
    }

    printf("Server listening on port 8080\n");

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);

        if (client_socket!= -1) {
            printf("Client connected: %d\n", client_socket);

            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket);

            if (num_clients < MAX_CLIENTS) {
                clients[num_clients++] = client_socket;
            }
        }
    }

    return 0;
}