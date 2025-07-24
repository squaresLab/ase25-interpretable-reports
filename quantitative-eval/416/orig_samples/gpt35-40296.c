//FormAI DATASET v1.0 Category: Simple Web Server ; Style: Linus Torvalds
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define QUEUE_SIZE 10
#define RESPONSE "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello World!"

void *handle_connection(void *arg){
    int client_sock = *(int*)arg;
    char buffer[1024] = {0};
    read(client_sock, buffer, 1024);
    write(client_sock, RESPONSE, strlen(RESPONSE));
    close(client_sock);
    pthread_exit(NULL);
}

int main(){
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock == -1){
        printf("Failed to create socket");
        return -1;
    }

    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if(bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        printf("Failed to bind");
        return -1;
    }

    if(listen(server_sock, QUEUE_SIZE) < 0){
        printf("Failed to listen");
        return -1;
    }

    printf("Listening on port %d...\n", PORT);

    while(1){
        socklen_t clilen = sizeof(client_addr);
        int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &clilen);
        if(client_sock < 0){
            printf("Failed to accept");
            continue;
        }
        pthread_t tid;
        if(pthread_create(&tid, NULL, handle_connection, &client_sock) != 0){
            printf("Failed to create thread");
            continue;
        }
        pthread_detach(tid);
    }

    close(server_sock);
    return 0;
}