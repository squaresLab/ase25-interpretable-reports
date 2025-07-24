//GEMINI-pro DATASET v1.0 Category: Building a FTP Client ; Style: protected
#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#define BACKLOG 10
#define MAX_CONNECTIONS 10
#define BUFFER_SIZE 1024

// Global variables
int server_socket;
struct sockaddr_in server_address;
struct sockaddr_in client_address;
socklen_t client_address_len;
int client_sockets[MAX_CONNECTIONS];
int num_connections;
pthread_t client_threads[MAX_CONNECTIONS];

// Function to handle client connections
void* handle_client(void* arg) {
    int client_socket = *(int*)arg;

    // Receive the request from the client
    char request[BUFFER_SIZE];
    int bytes_received = recv(client_socket, request, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        perror("recv");
        close(client_socket);
        pthread_exit(NULL);
    }

    // Parse the request
    char* command = strtok(request, " ");
    char* filename = strtok(NULL, " ");
    char* mode = strtok(NULL, " ");

    // Handle the request
    if (strcmp(command, "GET") == 0) {
        // Open the file
        int file_fd = open(filename, O_RDONLY);
        if (file_fd < 0) {
            perror("open");
            send(client_socket, "Error: Could not open file", strlen("Error: Could not open file"), 0);
            close(client_socket);
            pthread_exit(NULL);
        }

        // Send the file contents to the client
        char buffer[BUFFER_SIZE];
        int bytes_read;
        while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
            send(client_socket, buffer, bytes_read, 0);
        }
        if (bytes_read < 0) {
            perror("read");
            close(client_socket);
            pthread_exit(NULL);
        }

        // Close the file
        close(file_fd);
    } else if (strcmp(command, "PUT") == 0) {
        // Create the file
        int file_fd = open(filename, O_WRONLY | O_CREAT, 0644);
        if (file_fd < 0) {
            perror("open");
            send(client_socket, "Error: Could not create file", strlen("Error: Could not create file"), 0);
            close(client_socket);
            pthread_exit(NULL);
        }

        // Receive the file contents from the client
        char buffer[BUFFER_SIZE];
        int bytes_received;
        while ((bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0)) > 0) {
            write(file_fd, buffer, bytes_received);
        }
        if (bytes_received < 0) {
            perror("recv");
            close(client_socket);
            pthread_exit(NULL);
        }

        // Close the file
        close(file_fd);
    } else {
        // Send an error message to the client
        send(client_socket, "Error: Invalid command", strlen("Error: Invalid command"), 0);
        close(client_socket);
        pthread_exit(NULL);
    }

    // Close the client socket
    close(client_socket);

    // Exit the thread
    pthread_exit(NULL);
}

// Function to handle incoming connections
void handle_incoming_connections() {
    while (1) {
        // Accept a new connection
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        if (client_socket < 0) {
            perror("accept");
            continue;
        }

        // Add the client socket to the list of client sockets
        client_sockets[num_connections] = client_socket;
        num_connections++;

        // Create a new thread to handle the client connection
        pthread_create(&client_threads[num_connections], NULL, handle_client, &client_socket);
    }
}

// Function to handle Ctrl+C signal
void handle_sigint(int signum) {
    // Close the server socket
    close(server_socket);

    // Close all client sockets
    for (int i = 0; i < num_connections; i++) {
        close(client_sockets[i]);
    }

    // Exit the program
    exit(0);
}

int main(int argc, char* argv[]) {
    // Register the Ctrl+C signal handler
    signal(SIGINT, handle_sigint);

    // Create the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("socket");
        exit(1);
    }

    // Set the server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(21);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the server socket to the address
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("bind");
        close(server_socket);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, BACKLOG) < 0) {
        perror("listen");
        close(server_socket);
        exit(1);
    }

    // Handle incoming connections
    handle_incoming_connections();

    return 0;
}