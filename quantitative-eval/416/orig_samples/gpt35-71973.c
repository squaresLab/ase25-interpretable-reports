//FormAI DATASET v1.0 Category: Chat server ; Style: portable
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    int socket;
    struct sockaddr_in client_address;
} client_t;

client_t clients[MAX_CLIENTS]; // array to store all client connections
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // mutex for accessing shared variables

void *client_handler(void *arg) {
    int client_index = *(int*)arg;
    int client_socket = clients[client_index].socket;
    struct sockaddr_in client_address = clients[client_index].client_address;
    char buffer[BUFFER_SIZE];

    printf("New client connected: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

    while (1) {
        memset(buffer, 0, BUFFER_SIZE); // clear buffer
        int read_bytes = read(client_socket, buffer, BUFFER_SIZE); // receive message from client
        if (read_bytes <= 0) {
            // client has disconnected
            printf("Client disconnected: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
            close(client_socket);
            pthread_mutex_lock(&mutex);
            clients[client_index].socket = 0; // mark slot as available
            pthread_mutex_unlock(&mutex);
            break;
        } else {
            // broadcast message to all connected clients
            printf("Received message from %s:%d: %s\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), buffer);
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].socket > 0) {
                    int send_bytes = send(clients[i].socket, buffer, read_bytes, 0);
                    if (send_bytes < 0) {
                        printf("Error sending message to %s:%d\n", inet_ntoa(clients[i].client_address.sin_addr), ntohs(clients[i].client_address.sin_port));
                        continue;
                    }
                }
            }
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    int server_socket, client_socket, option = 1;
    struct sockaddr_in server_address, client_address;
    pthread_t threads[MAX_CLIENTS];

    memset(clients, 0, sizeof(clients)); // clear clients array

    // create server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printf("Error creating server socket\n");
        return EXIT_FAILURE;
    }

    // set server socket options
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
        printf("Error setting server socket options\n");
        return EXIT_FAILURE;
    }

    // initialize server address
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080);

    // bind server socket to address
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        printf("Error binding server socket to address\n");
        return EXIT_FAILURE;
    }

    // listen for incoming client connections
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        printf("Error listening for incoming client connections\n");
        return EXIT_FAILURE;
    }

    printf("Server is listening on port %d\n", ntohs(server_address.sin_port));

    while (1) {
        // accept new client connection
        socklen_t client_address_size = sizeof(client_address);
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size)) == -1) {
            printf("Error accepting new client connection\n");
            continue;
        }

        // find first available slot in clients array
        int client_index = -1;
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].socket == 0) {
                client_index = i;
                break;
            }
        }

        if (client_index == -1) {
            // maximum number of clients already connected
            printf("Maximum number of clients already connected\n");
            close(client_socket);
        } else {
            // add new client to clients array
            clients[client_index].socket = client_socket;
            clients[client_index].client_address = client_address;

            // create new thread to handle client
            if (pthread_create(&threads[client_index], NULL, client_handler, &client_index) != 0) {
                printf("Error creating thread to handle client\n");
                close(client_socket);
                clients[client_index].socket = 0; // mark slot as available
            }
        }
        pthread_mutex_unlock(&mutex);
    }

    return EXIT_SUCCESS;
}