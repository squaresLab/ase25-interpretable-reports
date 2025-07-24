//GEMINI-pro DATASET v1.0 Category: Simple Web Server ; Style: multi-threaded
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 8080

// Global variables
int server_fd;
struct sockaddr_in server_addr;
pthread_mutex_t mutex;

// Thread function to handle client requests
void *handle_client(void *arg) {
    // Get the client socket descriptor
    int client_fd = *(int *)arg;
    
    // Lock the mutex to ensure exclusive access to global variables
    pthread_mutex_lock(&mutex);
    
    // Read the HTTP request from the client
    char request[1024];
    int bytes_read = read(client_fd, request, sizeof(request));
    if (bytes_read == 0) {
        // Client closed the connection
        printf("Client closed the connection\n");
    } else if (bytes_read < 0) {
        // Error reading the request
        perror("read");
    } else {
        // Parse the HTTP request
        char *method = strtok(request, " ");
        char *path = strtok(NULL, " ");
        char *version = strtok(NULL, "\r\n");
        
        // Check if the request is valid
        if (strcmp(method, "GET") != 0) {
            // Invalid method
            printf("Invalid method: %s\n", method);
            write(client_fd, "HTTP/1.1 405 Method Not Allowed\r\n\r\n", 35);
        } else if (strcmp(version, "HTTP/1.1") != 0) {
            // Invalid version
            printf("Invalid version: %s\n", version);
            write(client_fd, "HTTP/1.1 505 HTTP Version Not Supported\r\n\r\n", 45);
        } else {
            // Valid request
            
            // Get the file path
            char file_path[1024];
            if (strcmp(path, "/") == 0) {
                strcpy(file_path, "index.html");
            } else {
                strcpy(file_path, path);
            }
            
            // Open the file
            FILE *file = fopen(file_path, "r");
            if (file == NULL) {
                // File not found
                printf("File not found: %s\n", file_path);
                write(client_fd, "HTTP/1.1 404 Not Found\r\n\r\n", 29);
            } else {
                // File found
                
                // Get the file size
                fseek(file, 0, SEEK_END);
                int file_size = ftell(file);
                fseek(file, 0, SEEK_SET);
                
                // Send the HTTP response header
                char header[1024];
                sprintf(header, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n", file_size);
                write(client_fd, header, strlen(header));
                
                // Send the file content
                char buffer[1024];
                while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
                    write(client_fd, buffer, bytes_read);
                }
                
                // Close the file
                fclose(file);
            }
        }
    }
    
    // Unlock the mutex
    pthread_mutex_unlock(&mutex);
    
    // Close the client socket
    close(client_fd);
    
    return NULL;
}

int main() {
    // Create the server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }
    
    // Set the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind the server socket to the server address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        return EXIT_FAILURE;
    }
    
    // Listen for client connections
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        return EXIT_FAILURE;
    }
    
    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);
    
    // Main loop
    while (1) {
        // Accept a client connection
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd == -1) {
            perror("accept");
            continue;
        }
        
        // Create a new thread to handle the client request
        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, &client_fd) != 0) {
            perror("pthread_create");
            close(client_fd);
            continue;
        }
    }
    
    // Close the server socket
    close(server_fd);
    
    // Destroy the mutex
    pthread_mutex_destroy(&mutex);
    
    return EXIT_SUCCESS;
}