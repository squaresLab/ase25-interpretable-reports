//GEMINI-pro DATASET v1.0 Category: Simple Web Server ; Style: multi-threaded
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

// Function to handle client requests
void *handle_client(void *args)
{
    int sockfd = *(int *)args;
    char buffer[1024];

    // Receive the request from the client
    int n = read(sockfd, buffer, sizeof(buffer));
    if (n < 0)
    {
        perror("read");
        close(sockfd);
        pthread_exit(NULL);
    }

    // Parse the request to get the requested URL
    char *url = strtok(buffer, " ");
    if (!url)
    {
        perror("strtok");
        close(sockfd);
        pthread_exit(NULL);
    }

    // Open the requested file
    FILE *fp = fopen(url + 1, "rb");
    if (!fp)
    {
        perror("fopen");
        close(sockfd);
        pthread_exit(NULL);
    }

    // Send the file contents to the client
    int bytes_sent;
    while ((bytes_sent = fread(buffer, 1, sizeof(buffer), fp)) > 0)
    {
        if (write(sockfd, buffer, bytes_sent) < 0)
        {
            perror("write");
            close(sockfd);
            fclose(fp);
            pthread_exit(NULL);
        }
    }

    // Close the socket and the file
    close(sockfd);
    fclose(fp);

    pthread_exit(NULL);
}

int main()
{
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the port
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for client connections
    if (listen(sockfd, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accept client connections and handle them in separate threads
    while (1)
    {
        int clientfd = accept(sockfd, NULL, NULL);
        if (clientfd < 0)
        {
            perror("accept");
            continue;
        }

        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, &clientfd) != 0)
        {
            perror("pthread_create");
            close(clientfd);
            continue;
        }

        pthread_detach(tid);
    }

    // Close the listening socket
    close(sockfd);

    return 0;
}