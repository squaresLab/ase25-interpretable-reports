//GEMINI-pro DATASET v1.0 Category: Appointment Scheduler ; Style: decentralized
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10

typedef struct {
    int fd;
    char name[256];
} client_t;

client_t clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg) {
    int fd = *(int *)arg;
    char buffer[256];
    int n;

    while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
        // Process the request
        if (strncmp(buffer, "PING", 4) == 0) {
            // Send a PONG response
            write(fd, "PONG", 4);
        } else if (strncmp(buffer, "BOOK", 4) == 0) {
            // Parse the appointment details
            char *name = strtok(buffer, " ");
            char *date = strtok(NULL, " ");
            char *time = strtok(NULL, " ");

            // Check if the client is already registered
            pthread_mutex_lock(&clients_mutex);
            int i;
            for (i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].fd == fd) {
                    strcpy(clients[i].name, name);
                    break;
                }
            }
            pthread_mutex_unlock(&clients_mutex);

            // Book the appointment
            printf("Booked appointment for %s on %s at %s\n", name, date, time);

            // Send a confirmation message
            write(fd, "OK", 2);
        }
    }

    // Close the socket
    close(fd);

    return NULL;
}

int main() {
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    // Bind the socket to the port
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr = { .s_addr = INADDR_ANY },
    };

    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(sockfd, MAX_CLIENTS) == -1) {
        perror("listen");
        exit(1);
    }

    // Accept incoming connections
    while (1) {
        int fd = accept(sockfd, NULL, NULL);
        if (fd == -1) {
            perror("accept");
            continue;
        }

        // Create a new thread to handle the client
        pthread_t thread;
        pthread_create(&thread, NULL, handle_client, &fd);
    }

    // Close the socket
    close(sockfd);

    return 0;
}