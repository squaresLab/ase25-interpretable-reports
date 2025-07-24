//FormAI DATASET v1.0 Category: Client Server Application ; Style: immersive
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080
#define BUFF_SIZE 1024

void *connection_handler(void *);

int main(int argc, char const *argv[]) {

    //create socket
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");

    //prepare socket address
    struct sockaddr_in server, client;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    //bind socket to the port
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) == -1) {
        printf("Bind failed");
        return 1;
    }
    puts("Bind done");

    //listen for incoming connections
    listen(socket_desc, 3);

    //keep accepting incoming connections
    puts("Waiting for incoming connections...");
    int c = sizeof(struct sockaddr_in);
    int new_socket;
    while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c))) {
        puts("Connection accepted");

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, connection_handler, (void *)&new_socket) < 0) {
            perror("Could not create thread");
            return 1;
        }

        //close socket file descriptor
        close(new_socket);
    }

    if (new_socket < 0) {
        perror("Accept failed");
        return 1;
    }

    return 0;
}

void *connection_handler(void *socket_desc) {
    //get socket descriptor
    int sock = *(int *)socket_desc;
    char buffer[BUFF_SIZE];
    int read_size;

    //send initial message to client
    char *message = "Welcome to the Chat Server! Type 'bye' to exit\n";
    write(sock, message, strlen(message));

    //keep communication going with client
    while ((read_size = recv(sock, buffer, BUFF_SIZE, 0)) > 0) {
        //add null terminator to end of message
        buffer[read_size] = '\0';

        //check if client wants to exit
        if (strncmp(buffer, "bye", 3) == 0) {
            message = "Goodbye!\n";
            write(sock, message, strlen(message));
            break;
        }

        //print message from client to server console
        printf("Client: %s", buffer);

        //send message back to client
        write(sock, buffer, strlen(buffer));
    }

    if (read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    } else if (read_size == -1) {
        perror("Receive failed");
    }

    //exit thread
    pthread_exit(NULL);
}