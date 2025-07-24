//GEMINI-pro DATASET v1.0 Category: Chat server ; Style: automated
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

// Default port number
#define PORT 8080

// Maximum number of clients
#define MAX_CLIENTS 10

// Maximum message length
#define MAX_MSG_LEN 1024

// Client structure
typedef struct client {
    int sockfd;
    char name[MAX_MSG_LEN];
} client_t;

// Client list
client_t clients[MAX_CLIENTS];

// Mutex to protect the client list
pthread_mutex_t client_list_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to handle client connections
void *handle_client(void *arg) {
    // Get the client socket
    int sockfd = *(int *)arg;

    // Get the client name
    char name[MAX_MSG_LEN];
    recv(sockfd, name, sizeof(name), 0);

    // Add the client to the client list
    pthread_mutex_lock(&client_list_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].sockfd == 0) {
            clients[i].sockfd = sockfd;
            strcpy(clients[i].name, name);
            break;
        }
    }
    pthread_mutex_unlock(&client_list_mutex);

    // Send a welcome message to the client
    char welcome_msg[MAX_MSG_LEN];
    sprintf(welcome_msg, "Welcome to the chat server, %s!", name);
    send(sockfd, welcome_msg, strlen(welcome_msg), 0);

    // Loop to handle client messages
    while (1) {
        // Receive a message from the client
        char msg[MAX_MSG_LEN];
        int recv_len = recv(sockfd, msg, sizeof(msg), 0);

        // Check if the client has disconnected
        if (recv_len == 0) {
            pthread_mutex_lock(&client_list_mutex);
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].sockfd == sockfd) {
                    clients[i].sockfd = 0;
                    break;
                }
            }
            pthread_mutex_unlock(&client_list_mutex);
            break;
        }

        // Send the message to all other clients
        pthread_mutex_lock(&client_list_mutex);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].sockfd != sockfd && clients[i].sockfd != 0) {
                send(clients[i].sockfd, msg, recv_len, 0);
            }
        }
        pthread_mutex_unlock(&client_list_mutex);
    }

    // Close the client socket
    close(sockfd);

    return NULL;
}

int main() {
    // Create a server socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    // Set the socket options
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        return EXIT_FAILURE;
    }

    // Bind the socket to the port
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return EXIT_FAILURE;
    }

    // Listen for connections
    if (listen(sockfd, MAX_CLIENTS) < 0) {
        perror("listen");
        return EXIT_FAILURE;
    }

    // Loop to accept client connections
    while (1) {
        // Accept a client connection
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sockfd < 0) {
            perror("accept");
            continue;
        }

        // Create a new thread to handle the client connection
        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, (void *)&client_sockfd) != 0) {
            perror("pthread_create");
            close(client_sockfd);
        }
    }

    // Close the server socket
    close(sockfd);

    return EXIT_SUCCESS;
}