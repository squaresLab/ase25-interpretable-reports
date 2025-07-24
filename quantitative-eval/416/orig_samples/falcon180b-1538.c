//Falcon-180B DATASET v1.0 Category: Wi-Fi Signal Strength Analyzer ; Style: multi-threaded
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define PORT 8080

int sock;
struct sockaddr_in server_addr;
int addrlen;
int client_socks[MAX_CLIENTS];
int num_clients;

void *handle_client(void *arg) {
    int client_sock = *(int*) arg;
    char buffer[BUFFER_SIZE];
    while(1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_sock, buffer, BUFFER_SIZE, 0);
        if(bytes_received <= 0) {
            close(client_sock);
            pthread_exit(0);
        }
        printf("Client %d sent: %s\n", client_sock, buffer);
    }
}

int main() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        printf("Error creating socket\n");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        printf("Error binding socket\n");
        return 1;
    }

    if(listen(sock, MAX_CLIENTS) == -1) {
        printf("Error listening on socket\n");
        return 1;
    }

    addrlen = sizeof(struct sockaddr_in);
    puts("Server is listening on port 8080...");

    while(1) {
        int client_sock = accept(sock, (struct sockaddr*)&server_addr, (socklen_t*)&addrlen);
        if(client_sock == -1) {
            printf("Error accepting connection\n");
            continue;
        }

        if(num_clients >= MAX_CLIENTS) {
            close(client_sock);
            printf("Maximum number of clients reached\n");
            continue;
        }

        client_socks[num_clients] = client_sock;
        num_clients++;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void*)&client_sock);
    }

    return 0;
}