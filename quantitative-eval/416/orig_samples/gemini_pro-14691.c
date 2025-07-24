//GEMINI-pro DATASET v1.0 Category: Networking ; Style: asynchronous
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 8080
#define MAXLINE 1024

// Define the function to be executed by the thread
void *handle_client(void *arg)
{
    // Get the socket descriptor for the client
    int sockfd = *(int *)arg;

    // Read the message from the client
    char buffer[MAXLINE];
    int n = read(sockfd, buffer, MAXLINE);
    if (n < 0)
    {
        perror("Error reading from client");
        return NULL;
    }

    // Process the message from the client
    // ...

    // Write the response to the client
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
    {
        perror("Error writing to client");
        return NULL;
    }

    // Close the socket for the client
    close(sockfd);

    // Return NULL to indicate that the thread has finished executing
    return NULL;
}

int main()
{
    // Create a socket for the server
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return -1;
    }

    // Set the socket options
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("Error setting socket options");
        return -1;
    }

    // Bind the socket to the port
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Error binding socket to port");
        return -1;
    }

    // Listen for incoming connections
    if (listen(sockfd, 5) < 0)
    {
        perror("Error listening for incoming connections");
        return -1;
    }

    // Main loop
    while (1)
    {
        // Accept incoming connections
        struct sockaddr_in cliaddr;
        socklen_t len = sizeof(cliaddr);
        int clientfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);
        if (clientfd < 0)
        {
            perror("Error accepting incoming connection");
            continue;
        }

        // Create a new thread to handle the client
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, &clientfd) != 0)
        {
            perror("Error creating thread");
            close(clientfd);
            continue;
        }

        // Detach the thread so that it can run independently
        pthread_detach(thread);
    }

    // Close the socket for the server
    close(sockfd);

    return 0;
}