//FormAI DATASET v1.0 Category: Client Server Application ; Style: Cyberpunk
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *connection_handler(void *);

int main()
{
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Listening on port %d\n", PORT);

    while (1)
    {
        int new_socket;
        struct sockaddr_in client_address;
        char *client_ip;

        if ((new_socket = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        // Convert client IP to string format
        client_ip = inet_ntoa(client_address.sin_addr);

        printf("New connection from %s:%d\n", client_ip, ntohs(client_address.sin_port));

        pthread_t tid;
        if (pthread_create(&tid, NULL, connection_handler, (void *)&new_socket) < 0)
        {
            perror("pthread_create failed");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

void *connection_handler(void *socket_ptr)
{
    int socket = *(int *)socket_ptr;
    int valread;
    char buffer[1024] = {0};
    char response[1024] = {0};

    // Receive message from client
    valread = read(socket, buffer, 1024);
    printf("Received message from client: %s\n", buffer);

    // Generate response message
    sprintf(response, "Greetings, Cyberpunk! Your message was %d bytes long.", valread);
    printf("Sending response message: %s\n", response);

    // Send response message to client
    pthread_mutex_lock(&mutex);
    write(socket, response, strlen(response));
    pthread_mutex_unlock(&mutex);

    // Close socket and exit connection handler thread
    close(socket);
    pthread_exit(NULL);
}