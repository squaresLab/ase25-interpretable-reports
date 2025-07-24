//GEMINI-pro DATASET v1.0 Category: Antivirus scanner ; Style: multiplayer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 4444
#define BUFFER_SIZE 1024

// Antivirus scanner function
int scan_file(char *filename) {
    // Open the file
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("fopen");
        return -1;
    }

    // Read the file into a buffer
    int size = 1000000;
    char *buffer = malloc(size);
    int bytes_read = fread(buffer, 1, size, fp);

    // Close the file
    fclose(fp);

    // Scan the buffer for viruses
    int num_viruses = 0;
    for (int i = 0; i < bytes_read; i++) {
        if (buffer[i] == 'V') {
            num_viruses++;
        }
    }

    // Free the buffer
    free(buffer);

    // Return the number of viruses found
    return num_viruses;
}

// Thread function
void *thread_func(void *arg) {
    // Get the socket descriptor
    int sockfd = *(int *)arg;

    // Receive the filename from the client
    char filename[BUFFER_SIZE];
    int bytes_received = recv(sockfd, filename, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        perror("recv");
        close(sockfd);
        pthread_exit(NULL);
    }

    // Scan the file for viruses
    int num_viruses = scan_file(filename);

    // Send the number of viruses found to the client
    int bytes_sent = send(sockfd, &num_viruses, sizeof(int), 0);
    if (bytes_sent < 0) {
        perror("send");
        close(sockfd);
        pthread_exit(NULL);
    }

    // Close the socket
    close(sockfd);

    // Exit the thread
    pthread_exit(NULL);
}

int main() {
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    // Set the socket options
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(sockfd);
        return -1;
    }

    // Bind the socket to the port
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        close(sockfd);
        return -1;
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0) {
        perror("listen");
        close(sockfd);
        return -1;
    }

    // Accept incoming connections and create threads for each client
    while (1) {
        int clientfd = accept(sockfd, NULL, NULL);
        if (clientfd < 0) {
            perror("accept");
            continue;
        }

        // Create a new thread for the client
        pthread_t thread;
        if (pthread_create(&thread, NULL, thread_func, &clientfd) != 0) {
            perror("pthread_create");
            close(clientfd);
            continue;
        }
    }

    // Close the socket
    close(sockfd);

    return 0;
}