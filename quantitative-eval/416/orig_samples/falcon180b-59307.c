//Falcon-180B DATASET v1.0 Category: Networking ; Style: decentralized
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080
#define MAX 100

int sock;
int addrlen = sizeof(struct sockaddr_in);
struct sockaddr_in serv_addr, cli_addr;

void *thread(void *socket) {
    int sock = *((int *) socket);
    char buffer[MAX];

    while(1) {
        bzero(buffer, MAX);
        int bytes_recv = recv(sock, buffer, MAX, 0);
        if(bytes_recv <= 0) {
            break;
        }
        printf("Thread %d received message: %s\n", sock, buffer);
    }

    close(sock);
    pthread_exit(0);
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        printf("Error: Unable to create socket\n");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if(bind(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("Error: Unable to bind socket\n");
        exit(1);
    }

    if(listen(sock, 5) < 0) {
        printf("Error: Unable to listen on socket\n");
        exit(1);
    }

    printf("Server is listening on port %d\n", PORT);

    while(1) {
        socklen_t addr_size = sizeof(cli_addr);
        int new_sock = accept(sock, (struct sockaddr *) &cli_addr, &addr_size);
        if(new_sock < 0) {
            printf("Error: Unable to accept connection\n");
            exit(1);
        }

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, thread, (void *) &new_sock);
    }

    return 0;
}