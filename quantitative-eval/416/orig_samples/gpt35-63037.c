//FormAI DATASET v1.0 Category: Networking ; Style: futuristic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080
#define MAX_MSG_LENGTH 1024

// Client struct to hold socket info
struct client {
    int sockfd;
    char name[50];
};

// Function to handle messages sent to all clients
void *sendToAll(void *args) {
    struct client *cl = (struct client *) args;
    char msg[MAX_MSG_LENGTH];
    while (1) {
        fgets(msg, MAX_MSG_LENGTH, stdin);
        msg[strcspn(msg, "\n")] = '\0'; // remove newline character
        sprintf(msg, "%s: %s", cl->name, msg);
        if (send(cl->sockfd, msg, strlen(msg), 0) < 0) {
            perror("Error sending message to all clients");
        }
    }
}

// Function to handle messages received from specific client
void *recvFromClient(void *args) {
    struct client *cl = (struct client *) args;
    char inBuffer[MAX_MSG_LENGTH];
    while (1) {
        int valRead = read(cl->sockfd, inBuffer, MAX_MSG_LENGTH);
        if (valRead == 0) {
            printf("Client %s disconnected\n", cl->name);
            close(cl->sockfd);
            pthread_exit(NULL);
        }
        inBuffer[valRead] = '\0';
        printf("%s\n", inBuffer);
    }
}

int main(int argc, char const *argv[]) {
    int serverfd, newSocket, valRead;
    struct sockaddr_in serverAddr;
    struct client clientList[10];
    pthread_t sendThread;

    // Create socket
    serverfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverfd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Setup server address and bind socket
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(serverfd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(serverfd, 10) < 0) {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);
    int clientCount = 0;

    // Loop for accepting new clients and handling messages
    while (1) {
        // Accept new connections
        newSocket = accept(serverfd, (struct sockaddr *) NULL, NULL);
        if (newSocket < 0) {
            perror("Error accepting connection");
            exit(EXIT_FAILURE);
        }

        // Prompt client for name and add to client list
        struct client cl;
        cl.sockfd = newSocket;
        printf("Enter your name: ");
        fgets(cl.name, 50, stdin);
        cl.name[strcspn(cl.name, "\n")] = '\0'; // remove newline character
        clientList[clientCount] = cl;
        clientCount++;

        // Create thread for receiving messages from client
        pthread_t recvThread;
        if (pthread_create(&recvThread, NULL, recvFromClient, (void *) &cl) != 0) {
            perror("Error creating thread");
            exit(EXIT_FAILURE);
        }

        // Start thread for sending message to all clients
        if (clientCount == 1) { // start thread only once
            if (pthread_create(&sendThread, NULL, sendToAll, (void *) &cl) != 0) {
                perror("Error creating thread");
                exit(EXIT_FAILURE);
            }
        }
    }

    return 0;
}