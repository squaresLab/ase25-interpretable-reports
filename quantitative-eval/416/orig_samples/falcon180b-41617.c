//Falcon-180B DATASET v1.0 Category: Smart home automation ; Style: scalable
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define MAX_CLIENTS 10
#define PORT 8080
#define THREAD_NUM 10

int client_sock[MAX_CLIENTS];
int client_num = 0;
int thread_id[THREAD_NUM];
pthread_t thread[THREAD_NUM];

void* handle_client(void* arg) {
    int client_socket = *((int*) arg);
    char buffer[BUF_SIZE];
    while(1) {
        bzero(buffer, BUF_SIZE);
        int ret = recv(client_socket, buffer, BUF_SIZE, 0);
        if(ret <= 0) {
            break;
        }
        printf("Client %d says: %s\n", client_socket, buffer);
        for(int i=0; i<client_num; i++) {
            if(client_sock[i]!= client_socket) {
                send(client_sock[i], buffer, strlen(buffer), 0);
            }
        }
    }
    close(client_socket);
    pthread_exit(0);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);

    printf("Server started on port %d\n", PORT);

    while(1) {
        int client_socket = accept(server_socket, NULL, NULL);
        printf("Client %d connected\n", client_socket);
        if(client_num >= MAX_CLIENTS) {
            send(client_socket, "Server is full\n", 15, 0);
            close(client_socket);
        } else {
            client_sock[client_num] = client_socket;
            pthread_create(&thread[client_num], NULL, handle_client, (void*)&client_socket);
            client_num++;
        }
    }

    return 0;
}