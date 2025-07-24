//FormAI DATASET v1.0 Category: Chat server ; Style: shocked
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 3000
#define MAX_CLIENTS 5
#define BUFFER_SIZE 1024

int active_clients = 0;
int client_sockets[MAX_CLIENTS];
char buffer[BUFFER_SIZE];

void send_message_to_clients(char *message, int sender_socket) {
    for (int i = 0; i < active_clients; i++) {
        int client_socket = client_sockets[i];
        if (client_socket != sender_socket) {
            send(client_socket, message, strlen(message), 0);
        }
    }
}

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    printf("New client connected: %d\n", client_socket);
    sprintf(buffer, "Welcome to the chat room, client %d\n", client_socket);
    send(client_socket, buffer, strlen(buffer), 0);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received == 0) {
            printf("Client %d disconnected\n", client_socket);
            sprintf(buffer, "Client %d has left the chat\n", client_socket);
            send_message_to_clients(buffer, client_socket);
            break;
        }

        sprintf(buffer, "Client %d: %s", client_socket, buffer);
        send_message_to_clients(buffer, client_socket);
    }

    active_clients--;
    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int bind_result = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (bind_result == -1) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    int listen_result = listen(server_socket, MAX_CLIENTS);
    if (listen_result == -1) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Server started listening on port %d...\n", PORT);

    struct sockaddr_in client_address;
    socklen_t client_size = sizeof(client_address);

    while (1) {
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_size);
        if (client_socket == -1) {
            perror("Client connection failed");
            continue;
        }

        if (active_clients == MAX_CLIENTS) {
            sprintf(buffer, "The chat room is full, try again later\n");
            send(client_socket, buffer, strlen(buffer), 0);
            close(client_socket);
        } else {
            client_sockets[active_clients] = client_socket;
            active_clients++;

            pthread_t thread_id;
            int return_value = pthread_create(&thread_id, NULL, handle_client, &client_socket);
            if (return_value != 0) {
                perror("Thread creation failed");
                exit(EXIT_FAILURE);
            }

            printf("Thread created for client %d\n", client_socket);
        }
    }

    close(server_socket);

    return 0;
}