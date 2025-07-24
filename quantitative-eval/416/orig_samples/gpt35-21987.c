//FormAI DATASET v1.0 Category: Networking ; Style: content
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080

void *server_handler(void *arg);
void *client_handler(void *arg);

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the PORT
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the PORT
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Server is listening on port %d\n", PORT);

    // handle multiple client requests
    pthread_t thread_id;
    while ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)))
    {
        printf("New client connection accepted.\n");

        if (pthread_create(&thread_id, NULL, server_handler, (void *)&new_socket) < 0)
        {
            perror("Could not create thread.\n");
            return -1;
        }
    }

    return 0;
}

void *server_handler(void *arg)
{
    int sock = *(int *)arg;
    char buffer[1024] = {0};
    int valread;
    char *response = "Hello from server.";
    send(sock, response, strlen(response), 0);

    while ((valread = read(sock, buffer, 1024)) > 0)
    {
        printf("Received Message: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0)
        {
            char *bye_bye = "Goodbye! Hope to see you again.";
            send(sock, bye_bye, strlen(bye_bye), 0);
            printf("Client has disconnected.\n");
            close(sock);
            return NULL;
        }

        else
        {
            // Sending response to the client
            response = "Message received. Thank you.";
            send(sock, response, strlen(response), 0);
        }
        memset(buffer, 0, sizeof(buffer));
    }
    return NULL;
}