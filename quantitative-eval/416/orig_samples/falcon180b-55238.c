//Falcon-180B DATASET v1.0 Category: Intrusion detection system ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int clients[MAX_CLIENTS];
int clientCount = 0;

void *listenToClient(void *socket) {
    char buffer[BUFFER_SIZE];
    int clientSocket = *(int *)socket;

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);

        if (bytesReceived <= 0) {
            close(clientSocket);
            pthread_exit(0);
        }

        printf("Received message from client %d: %s\n", clientSocket, buffer);
    }
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 10);

    printf("Server started on port 8080\n");

    while (1) {
        int clientSocket = accept(serverSocket, NULL, NULL);

        if (clientSocket == -1) {
            printf("Error accepting connection\n");
            continue;
        }

        clientCount++;
        clients[clientCount - 1] = clientSocket;

        pthread_t threadID;
        pthread_create(&threadID, NULL, listenToClient, (void *)&clientSocket);
    }

    return 0;
}