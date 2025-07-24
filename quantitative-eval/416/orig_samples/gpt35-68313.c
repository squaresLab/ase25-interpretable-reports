//FormAI DATASET v1.0 Category: TCP/IP Programming ; Style: decentralized
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>

#define PORT 8080

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg){
    int client_sock = *((int *)arg);
    char buffer[1024] = {0};

    while(1){
        memset(buffer, 0, sizeof(buffer));
        int valread = read(client_sock, buffer, 1024);
        if(valread < 1){
            close(client_sock);
            return NULL;
        }
        printf("Client sent: %s", buffer);

        if(strcmp(buffer, "exit\n") == 0){
            close(client_sock);
            return NULL;
        }

        pthread_mutex_lock(&mutex);
        FILE *fp = fopen("messages.txt", "a");
        fprintf(fp, "Client %d: %s", client_sock, buffer);
        fclose(fp);
        pthread_mutex_unlock(&mutex);

        send(client_sock, buffer, strlen(buffer), 0);
    }
}

int main(){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Create server socket
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set server socket options
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind server socket to port
    if(bind(server_fd, (struct sockaddr *)&address, addrlen) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if(listen(server_fd, 5) < 0){
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server started listening on port %d...\n", PORT);

    // Handle incoming connections
    while((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))){
        printf("New client connected. Client ID: %d\n", new_socket);

        pthread_t thread_id;
        if(pthread_create(&thread_id, NULL, handle_client, (void *)&new_socket) < 0){
            perror("thread create failed");
            exit(EXIT_FAILURE);
        }
    }

    // Close server socket
    close(server_fd);
    return 0;
}