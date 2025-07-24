//FormAI DATASET v1.0 Category: Networking ; Style: thoughtful
/*This C program is a simple file transfer Client-Server application using socket programming in C. 
It is created in a manner where the client sends a file named 'test.txt' to the server where it is then saved in the same directory where the server program is located.*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define BUF_SIZE 1024

void *client_handler(void *);

int main(int argc, char *argv[]) {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    char buf[BUF_SIZE];

    //Create a socket
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error: Socket cannot be created");
        return EXIT_FAILURE;
    }

    //Specify server details
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    //Bind the socket to the server address and port
    if (bind(server_sock, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1) {
        perror("Error: Address binding failed");
        return EXIT_FAILURE;
    }

    //Listen for incoming requests
    if (listen(server_sock, 5) == -1) {
        perror("Error: Listen failed");
        return EXIT_FAILURE;
    }

    puts("Server waiting for incoming connections...");

    while (1) {
        socklen_t client_len = sizeof(client_addr);

        //accept incoming client connection
        if ((client_sock = accept(server_sock, (struct sockaddr*) &client_addr, &client_len)) == -1) {
            perror("Error: Accept failed");
            return EXIT_FAILURE;
        }

        puts("Connection Accepted");

        //Handle client message in a separate thread
        pthread_t handler_thread;
        if (pthread_create(&handler_thread, NULL, client_handler, (void*) &client_sock) < 0) {
            perror("Error: Thread creation failed");
            return EXIT_FAILURE;
        }
    }
    return 0;
}

void *client_handler(void *args) {
    int client_sock = *(int *)args;
    char filename[BUF_SIZE];
    char recv_buf[BUF_SIZE];

    //get the name of the file to be received from the client
    memset(filename, '\0', BUF_SIZE);
    if (recv(client_sock, filename, BUF_SIZE, 0) == -1) {
        perror("Error: File name receipt failed");
        return NULL;
    }

    //open file or create if not present
    FILE *fp;
    if ((fp = fopen(filename, "wb")) == NULL) {
        perror("Error: File creation failed");
        return NULL;
    }

    //Receive data from client and save it to file
    memset(recv_buf, '\0', BUF_SIZE);
    int bytes_received;
    while ((bytes_received = recv(client_sock, recv_buf, BUF_SIZE, 0)) > 0) {
        fwrite(recv_buf, 1, bytes_received, fp);
    }

    if (bytes_received == -1) {
        perror("Error: File download failed");
    } else {
        puts("File received successfully");
    }

    fclose(fp);
    close(client_sock);
    return NULL;
}