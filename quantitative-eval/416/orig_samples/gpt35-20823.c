//FormAI DATASET v1.0 Category: Stock market tracker ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 12345
#define BUFSIZE 1024
#define TICKERSIZE 10

void* handle_client(void* arg);

int main(int argc, char* argv[]) {

    // Create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        printf("Failed to create socket.\n");
        exit(EXIT_FAILURE);
    }

    // Configure address
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind to socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == -1) {
        printf("Failed to bind to socket.\n");
        exit(EXIT_FAILURE);
    }

    // Listen for clients
    if (listen(server_fd, 3) == -1) {
        printf("Failed to listen for clients.\n");
        exit(EXIT_FAILURE);
    }

    printf("Stock market tracker server listening on port %d...\n", PORT);

    // Handle clients in separate threads
    while (1) {
        int client_fd = accept(server_fd, (struct sockaddr*)NULL, NULL);
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, handle_client, (void*)&client_fd) != 0) {
            printf("Failed to create thread for client.\n");
            exit(EXIT_FAILURE);
        }
    }

    close(server_fd);
    return 0;
}

void* handle_client(void* arg) {
    int client_fd = *(int*)arg;

    char buffer[BUFSIZE] = {0};
    int valread = read(client_fd, buffer, BUFSIZE);

    if (valread == -1) {
        printf("Failed to read from client.\n");
        exit(EXIT_FAILURE);
    }

    char ticker[TICKERSIZE] = {0};
    strncpy(ticker, buffer, TICKERSIZE);
    printf("Received request for stock: %s\n", ticker);

    // TODO - Query stock market API for stock data and send back to client

    close(client_fd);
    pthread_exit(NULL);
}