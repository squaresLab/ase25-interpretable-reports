//FormAI DATASET v1.0 Category: Networking ; Style: genious
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080

int clientCount = 0;
int clientSockets[10];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *clientHandler(void *arg) {
    int clientSocket = *(int *) arg;
    int bufferLength = 1024;
    char buffer[bufferLength];
    
    pthread_mutex_lock(&mutex);
    clientCount++;
    pthread_mutex_unlock(&mutex);
    
    while (1) {
        bzero(buffer, bufferLength);
        int read = recv(clientSocket, buffer, bufferLength, 0);
        
        if (read == 0) {
            pthread_mutex_lock(&mutex);
            for (int i = 0; i < clientCount; i++) {
                if (clientSockets[i] == clientSocket) {
                    while (i < clientCount - 1) {
                        clientSockets[i] = clientSockets[i + 1];
                        i++;
                    }
                    break;
                }
            }
            clientCount--;
            pthread_mutex_unlock(&mutex);
            printf("Client with socket %d disconnected\n", clientSocket);
            break;
        }
        
        printf("Received message from client %d: %s\n", clientSocket, buffer);
        
        for (int i = 0; i < clientCount; i++) {
            if (clientSockets[i] != clientSocket) {
                send(clientSockets[i], buffer, strlen(buffer), 0);
            }
        }
    }
    
    close(clientSocket);
    pthread_exit(NULL);
}

int main() {
    int serverSocket;
    struct sockaddr_in serverAddress;
    pthread_t threadId;
    
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        printf("Failed to create socket\n");
        exit(EXIT_FAILURE);
    }
    
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);
    
    if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
        printf("Failed to bind socket\n");
        exit(EXIT_FAILURE);
    }
    
    if (listen(serverSocket, 10) == -1) {
        printf("Failed to listen on socket\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port %d\n", PORT);
    
    while (1) {
        struct sockaddr_in clientAddress;
        int clientSocket, clientAddressLength = sizeof(clientAddress);
        
        clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, (socklen_t *) &clientAddressLength);
        if (clientSocket == -1) {
            printf("Failed to accept client connection\n");
            continue;
        }
        
        if (clientCount == 10) {
            printf("Max clients reached, no more connections allowed\n");
            close(clientSocket);
            continue;
        }
        
        pthread_mutex_lock(&mutex);
        clientSockets[clientCount] = clientSocket;
        pthread_mutex_unlock(&mutex);
        
        printf("Client with socket %d connected\n", clientSocket);
        
        if (pthread_create(&threadId, NULL, &clientHandler, &clientSocket) != 0) {
            printf("Failed to create client thread\n");
            continue;
        }
    }
    
    close(serverSocket);
    pthread_exit(NULL);
}