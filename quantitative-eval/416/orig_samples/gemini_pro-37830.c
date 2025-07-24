//GEMINI-pro DATASET v1.0 Category: Socket programming ; Style: happy
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#define PORT 8080

// A happy greeting to send clients
const char *greeting = "Hello, world! We're glad you're here :)\n";

// This thread function handles communication with individual clients
void *handle_client(void *arg) {
    int sockfd = *(int *)arg;

    // Receive the client's message
    char buffer[1024];
    int n = read(sockfd, buffer, sizeof(buffer));

    // Print the client's message
    printf("Client: %s", buffer);

    // Send the greeting to the client
    write(sockfd, greeting, strlen(greeting));

    // Close the client socket
    close(sockfd);

    return NULL;
}

int main() {
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to an address
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(sockfd, 5) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accept connections and spawn threads to handle them
    while (1) {
        // Accept a connection
        int newsockfd = accept(sockfd, NULL, NULL);
        if (newsockfd == -1) {
            perror("accept");
            continue;
        }

        // Create a thread to handle the connection
        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, &newsockfd);
        pthread_detach(thread);
    }

    // Close the socket
    close(sockfd);

    return 0;
}