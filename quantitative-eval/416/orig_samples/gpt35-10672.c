//FormAI DATASET v1.0 Category: Networking ; Style: expert-level
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 5555

int client_count = 0;
int socket_desc;
struct sockaddr_in server_addr;
pthread_mutex_t mutex;

void *connection_handler(void *);

int main(int argc, char *argv[]) {
    // Creating socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_desc == -1) {
        printf("Could not create socket");
        return 1;
    }

    // Assigning IP and PORT
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(socket_desc, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        printf("Could not bind socket");
        return 1;
    }
    printf("Socket successfully bound\n");

    // Listen for incoming connections
    if (listen(socket_desc, 3) < 0) {
        printf("Error occurred while listening");
        return 1;
    }

    printf("Server listening at port %d\n", PORT);

    // Accept incoming connections
    struct sockaddr_in client_addr;
    int client_socket;
    pthread_t thread_id;

    while ((client_socket = accept(socket_desc, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr)) ) {
        printf("Connection accepted\n");

        pthread_mutex_lock(&mutex);
        client_count++;
        pthread_mutex_unlock(&mutex);

        // Create thread for handling connection
        if (pthread_create(&thread_id, NULL, connection_handler, (void *) &client_socket) < 0) {
            printf("Could not create thread");
            return 1;
        }
    }

    if (client_socket < 0) {
        printf("Error occurred while accepting connection");
        return 1;
    }

    return 0;
}

void *connection_handler(void *socket_desc) {
    int client_socket = *(int *) socket_desc;
    int read_size;
    char *message, client_message[2000];

    // Send welcome message to client
    message = "Welcome to the server\n";
    write(client_socket, message, strlen(message));

    while ((read_size = recv(client_socket, client_message, 2000, 0)) > 0) {
        client_message[read_size] = '\0';
        printf("Client message: %s", client_message);

        // Send received message back to client
        write(client_socket, client_message, strlen(client_message));

        // Clear buffer
        memset(client_message, 0, sizeof(client_message));
    }

    pthread_mutex_lock(&mutex);
    client_count--;
    pthread_mutex_unlock(&mutex);

    // Close client socket
    close(client_socket);

    pthread_exit(NULL);
}