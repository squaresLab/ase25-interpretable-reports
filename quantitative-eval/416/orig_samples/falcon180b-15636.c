//Falcon-180B DATASET v1.0 Category: Client Server Application ; Style: multiplayer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define MAX_MESSAGE_SIZE 1024

int sockets[MAX_CLIENTS];
int num_clients = 0;
pthread_t threads[MAX_CLIENTS];

void *thread_function(void *param)
{
    int client_socket = *(int *)param;
    char buffer[MAX_MESSAGE_SIZE];

    while (1)
    {
        memset(buffer, 0, MAX_MESSAGE_SIZE);
        int bytes_received = recv(client_socket, buffer, MAX_MESSAGE_SIZE, 0);

        if (bytes_received <= 0)
        {
            close(client_socket);
            pthread_exit(0);
        }

        printf("Client %d sent message: %s\n", client_socket, buffer);
        send(client_socket, buffer, strlen(buffer), 0);
    }
}

void handle_client(int client_socket)
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, thread_function, (void *)&client_socket);
    num_clients++;
}

int main()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1)
    {
        printf("Error creating socket\n");
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        printf("Error binding socket\n");
        return 1;
    }

    if (listen(server_socket, MAX_CLIENTS) == -1)
    {
        printf("Error listening on socket\n");
        return 1;
    }

    printf("Server started on port %d\n", PORT);

    while (1)
    {
        int client_socket = accept(server_socket, NULL, NULL);

        if (client_socket == -1)
        {
            printf("Error accepting connection\n");
            continue;
        }

        if (num_clients >= MAX_CLIENTS)
        {
            close(client_socket);
            printf("Server full\n");
            continue;
        }

        handle_client(client_socket);
    }

    close(server_socket);
    return 0;
}