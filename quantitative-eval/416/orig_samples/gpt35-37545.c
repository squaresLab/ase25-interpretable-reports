//FormAI DATASET v1.0 Category: Client Server Application ; Style: mathematical
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

void *server_handler(void *args);
void *client_handler(void *args);

int main(int argc, char *argv[])
{
    int server_sock, client_sock, c;
    struct sockaddr_in server, client;

    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1)

    {
        printf("Could not create socket");
    }
    puts("Socket created");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Bind
    if (bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        // Print the error message
        perror("Bind failed. Error");
        return 1;
    }
    puts("Bind done");

    // Listen
    listen(server_sock, 3);

    // Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    while ((client_sock = accept(server_sock, (struct sockaddr *)&client, (socklen_t *)&c)))
    {

        puts("Connection accepted");

        pthread_t server_thread, client_thread;

        // Create handlers
        if (pthread_create(&server_thread, NULL, server_handler, (void *)&client_sock) < 0)
        {
            perror("Could not create server thread");
            return 1;
        }

        if (pthread_create(&client_thread, NULL, client_handler, (void *)&client_sock) < 0)
        {
            perror("Could not create client thread");
            return 1;
        }
    }

    if (client_sock < 0)
    {
        perror("Accept failed");
        return 1;
    }

    return 0;
}

/**
 * This will handle the server connection
 */
void *server_handler(void *args)
{
    int sock = *(int *)args;
    int read_size;
    char *message, client_message[2000];

    // Send message to the client
    message = "Welcome to my server. I am your server handler \n";
    write(sock, message, strlen(message));

    // Receive a message from client
    while ((read_size = recv(sock, client_message, 2000, 0)) > 0)
    {

        // Send the message back to the client
        write(sock, client_message, strlen(client_message));
    }

    if (read_size == 0)
    {
        // Print some client disconnected message
        puts("Client disconnected");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }

    // Free the socket pointer
    free(args);

    pthread_exit(NULL);
}

/**
 * This will handle the client connection
 */
void *client_handler(void *args)
{

    int sock = *(int *)args;
    int read_size;
    char *message, client_message[2000];

    // Send message to the client
    message = "Welcome to my server. I am your client handler \n";
    write(sock, message, strlen(message));

    // Receive a message from client
    while ((read_size = recv(sock, client_message, 2000, 0)) > 0)
    {

        // Send the message back to the client
        write(sock, client_message, strlen(client_message));
    }

    if (read_size == 0)
    {
        // Print some client disconnected message
        puts("Client disconnected");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }

    // Free the socket pointer
    free(args);

    pthread_exit(NULL);
}