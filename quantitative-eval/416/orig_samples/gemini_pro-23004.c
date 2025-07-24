//GEMINI-pro DATASET v1.0 Category: Client Server Application ; Style: standalone
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080

// Client thread function
void *client_thread(void *arg)
{
    int sockfd = *(int *)arg;
    char buffer[1024];

    // Receive a message from the server
    int n = recv(sockfd, buffer, sizeof(buffer), 0);
    if (n == -1)
    {
        perror("recv");
        exit(EXIT_FAILURE);
    }
    else if (n == 0)
    {
        printf("Server disconnected\n");
        close(sockfd);
        pthread_exit(NULL);
    }
    else
    {
        // Print the message received from the server
        printf("Received from server: %s\n", buffer);

        // Send a response to the server
        n = send(sockfd, "Hello from client", strlen("Hello from client") + 1, 0);
        if (n == -1)
        {
            perror("send");
            exit(EXIT_FAILURE);
        }
    }

    // Close the socket
    close(sockfd);

    return NULL;
}

// Server function
void *server_function(void *arg)
{
    int server_sockfd = *(int *)arg;

    // Listen for incoming connections
    int client_sockfd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    while ((client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &client_addr_len)) >= 0)
    {
        pthread_t client_thread_id;

        // Create a new thread to handle the client connection
        if (pthread_create(&client_thread_id, NULL, client_thread, &client_sockfd) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    if (client_sockfd == -1)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Close the server socket
    close(server_sockfd);

    return NULL;
}

int main(int argc, char **argv)
{
    // Create a socket
    int server_sockfd;
    if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to an address and port
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);
    if (bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_sockfd, 5) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Create a thread to handle the server function
    pthread_t server_thread_id;
    if (pthread_create(&server_thread_id, NULL, server_function, &server_sockfd) != 0)
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    // Join the server thread
    pthread_join(server_thread_id, NULL);

    // Close the server socket
    close(server_sockfd);

    return 0;
}