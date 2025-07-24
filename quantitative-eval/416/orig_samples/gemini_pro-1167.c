//GEMINI-pro DATASET v1.0 Category: Smart home automation ; Style: asynchronous
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

// Structure to store client information
typedef struct {
    int fd;
    struct sockaddr_in addr;
} client_t;

// List of connected clients
client_t clients[100];
int num_clients = 0;

// Thread function to handle client requests
void *handle_client(void *arg) {
    client_t *client = (client_t *)arg;
    char buf[1024];

    // Read data from client
    int n = read(client->fd, buf, sizeof(buf));
    if (n <= 0) {
        perror("read");
        close(client->fd);
        pthread_exit(NULL);
    }

    // Send data back to client
    n = write(client->fd, buf, n);
    if (n <= 0) {
        perror("write");
        close(client->fd);
        pthread_exit(NULL);
    }

    // Close client connection
    close(client->fd);
    pthread_exit(NULL);
}

int main() {
    // Create server socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(1);
    }

    // Set server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // Bind server socket to address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    // Listen for client connections
    if (listen(server_fd, 100) == -1) {
        perror("listen");
        exit(1);
    }

    // Main loop
    while (1) {
        // Accept client connection
        client_t client;
        socklen_t addrlen = sizeof(client.addr);
        client.fd = accept(server_fd, (struct sockaddr *)&client.addr, &addrlen);
        if (client.fd == -1) {
            perror("accept");
            continue;
        }

        // Add client to list of connected clients
        clients[num_clients++] = client;

        // Create thread to handle client requests
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, &client) != 0) {
            perror("pthread_create");
            close(client.fd);
            continue;
        }

        // Detach thread so that it will not be joined
        if (pthread_detach(thread) != 0) {
            perror("pthread_detach");
            close(client.fd);
            continue;
        }
    }

    // Close server socket
    close(server_fd);

    return 0;
}