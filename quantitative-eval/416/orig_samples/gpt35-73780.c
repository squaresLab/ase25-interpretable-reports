//FormAI DATASET v1.0 Category: Simple Web Server ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

void *handle_client(void *);

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to PORT
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connections, handle in another thread
    int client_socket;
    pthread_t thread_id;
    while ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)))
    {
        if (pthread_create(&thread_id, NULL, handle_client, &client_socket) < 0)
        {
            perror("Thread creation failed");
            exit(EXIT_FAILURE);
        }
    }

    if (client_socket < 0)
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}

void *handle_client(void *arg)
{
    int client_socket = *(int *)arg;
    char buffer[BUFFER_SIZE] = {0};
    FILE *file;
    char *filename;
    char *filetype;
    int fd;
    struct stat stat_buf;
    int ret;

    // Read request from client
    if (read(client_socket, buffer, BUFFER_SIZE) <= 0)
    {
        perror("Read failed");
        exit(EXIT_FAILURE);
    }

    // Get filename from request
    strtok(buffer, " ");
    filename = strtok(NULL, " ");
    filetype = strchr(filename, '.') + 1;

    // Open file for reading, send error message if file not found
    if ((fd = open(filename, O_RDONLY)) == -1)
    {
        sprintf(buffer, "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<html><body><h1>File not found</h1></body></html>");
        if (send(client_socket, buffer, strlen(buffer), 0) < 0)
        {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }
        close(client_socket);
        pthread_exit(NULL);
    }

    // Get file size and send HTTP response header
    fstat(fd, &stat_buf);
    sprintf(buffer, "HTTP/1.1 200 OK\r\nContent-Type: text/%s\r\nContent-Length: %lu\r\n\r\n", filetype, stat_buf.st_size);
    if (send(client_socket, buffer, strlen(buffer), 0) < 0)
    {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }

    // Send file contents
    while ((ret = read(fd, buffer, BUFFER_SIZE)) > 0)
    {
        if (send(client_socket, buffer, ret, 0) < 0)
        {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }
    }

    // Close file and socket, exit thread
    close(fd);
    close(client_socket);
    pthread_exit(NULL);
}