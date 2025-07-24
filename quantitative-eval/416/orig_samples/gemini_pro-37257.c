//GEMINI-pro DATASET v1.0 Category: Chat server ; Style: shape shifting
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>

// Define the maximum number of clients
#define MAX_CLIENTS 100

// Define the maximum message length
#define MAX_MSG_LEN 1024

// Define the port number
#define PORT 8080

// Define the server address
#define SERVER_ADDR "127.0.0.1"

// Define the server socket
int server_sock;

// Define the client sockets
int client_socks[MAX_CLIENTS];

// Define the number of clients
int num_clients = 0;

// Define the mutex
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Define the condition variable
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Define the thread pool
pthread_t thread_pool[MAX_CLIENTS];

// Define the thread function
void* thread_func(void* arg) {
    // Get the client socket
    int client_sock = *(int*)arg;

    // Get the client address
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    getpeername(client_sock, (struct sockaddr*)&client_addr, &client_addr_len);

    // Print the client address
    printf("Client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // Loop until the client disconnects
    while (1) {
        // Receive a message from the client
        char msg[MAX_MSG_LEN];
        int msg_len = recv(client_sock, msg, MAX_MSG_LEN, 0);

        // If the client disconnected, break the loop
        if (msg_len <= 0) {
            break;
        }

        // Print the message
        printf("Client: %s\n", msg);

        // Send a message to the client
        char reply[MAX_MSG_LEN];
        sprintf(reply, "Server: %s", msg);
        send(client_sock, reply, strlen(reply), 0);
    }

    // Close the client socket
    close(client_sock);

    // Decrement the number of clients
    pthread_mutex_lock(&mutex);
    num_clients--;
    pthread_mutex_unlock(&mutex);

    // Signal the condition variable
    pthread_cond_signal(&cond);

    // Return NULL
    return NULL;
}

// Define the main function
int main() {
    // Create the server socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    // Set the server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    // Bind the server socket to the server address
    bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Listen for incoming connections
    listen(server_sock, MAX_CLIENTS);

    // Loop until the server is stopped
    while (1) {
        // Accept an incoming connection
        int client_sock = accept(server_sock, (struct sockaddr*)NULL, NULL);

        // If the server is full, close the client socket
        if (num_clients >= MAX_CLIENTS) {
            close(client_sock);
            continue;
        }

        // Add the client socket to the list of client sockets
        client_socks[num_clients] = client_sock;

        // Increment the number of clients
        pthread_mutex_lock(&mutex);
        num_clients++;
        pthread_mutex_unlock(&mutex);

        // Create a thread to handle the client
        pthread_create(&thread_pool[num_clients], NULL, thread_func, &client_sock);
    }

    // Close the server socket
    close(server_sock);

    // Return 0
    return 0;
}