//GEMINI-pro DATASET v1.0 Category: Email Client ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_MESSAGE_LENGTH 1024
#define MAX_CONNECTIONS 10
#define MAX_THREADS 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t semaphore;

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;

    while (1) {
        char buffer[MAX_MESSAGE_LENGTH];
        int bytes_received = recv(client_socket, buffer, MAX_MESSAGE_LENGTH, 0);
        if (bytes_received <= 0) {
            sem_wait(&semaphore);
            pthread_mutex_lock(&mutex);
            printf("Client disconnected\n");
            close(client_socket);
            pthread_mutex_unlock(&mutex);
            sem_post(&semaphore);
            break;
        }

        pthread_mutex_lock(&mutex);
        printf("Received message: %s\n", buffer);
        pthread_mutex_unlock(&mutex);

        char response[MAX_MESSAGE_LENGTH];
        snprintf(response, MAX_MESSAGE_LENGTH, "Hello, client!");
        int bytes_sent = send(client_socket, response, strlen(response), 0);
        if (bytes_sent < 0) {
            sem_wait(&semaphore);
            pthread_mutex_lock(&mutex);
            printf("Failed to send message\n");
            pthread_mutex_unlock(&mutex);
            sem_post(&semaphore);
            break;
        }
    }

    pthread_exit(NULL);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(12345);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("bind");
        return EXIT_FAILURE;
    }

    if (listen(server_socket, MAX_CONNECTIONS) == -1) {
        perror("listen");
        return EXIT_FAILURE;
    }

    sem_init(&semaphore, 0, MAX_THREADS);

    pthread_t threads[MAX_THREADS];
    int num_threads = 0;
    while (1) {
        struct sockaddr_in client_address;
        socklen_t client_address_length = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_length);
        if (client_socket == -1) {
            perror("accept");
            continue;
        }

        if (num_threads < MAX_THREADS) {
            pthread_create(&threads[num_threads++], NULL, handle_client, &client_socket);
        } else {
            char response[MAX_MESSAGE_LENGTH];
            snprintf(response, MAX_MESSAGE_LENGTH, "Server is full, try again later");
            int bytes_sent = send(client_socket, response, strlen(response), 0);
            if (bytes_sent < 0) {
                perror("send");
            }
            close(client_socket);
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    close(server_socket);
    return EXIT_SUCCESS;
}