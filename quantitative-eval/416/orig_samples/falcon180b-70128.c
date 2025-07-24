//Falcon-180B DATASET v1.0 Category: System process viewer ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <pthread.h>

#define MAX 10

int globalSocket = -1;
int activeThreads = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *threadFunction(void *socketDescriptor) {
    int clientSocket = *((int *) socketDescriptor);
    char buffer[1024];
    while(1) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if(bytesRead <= 0) {
            break;
        }
        printf("Client %d sent: %s\n", clientSocket, buffer);
    }
    close(clientSocket);
    pthread_mutex_lock(&mutex);
    activeThreads--;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket == -1) {
        printf("Error creating socket\n");
        return 1;
    }

    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
        printf("Error binding socket\n");
        return 1;
    }

    if(listen(serverSocket, MAX) == -1) {
        printf("Error listening on socket\n");
        return 1;
    }

    printf("Server is listening on port 8080\n");

    while(1) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        if(clientSocket == -1) {
            printf("Error accepting connection\n");
            continue;
        }

        pthread_t threadId;
        pthread_create(&threadId, NULL, threadFunction, (void *)&clientSocket);

        pthread_mutex_lock(&mutex);
        activeThreads++;
        pthread_mutex_unlock(&mutex);
    }

    close(serverSocket);
    return 0;
}