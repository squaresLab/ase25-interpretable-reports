//FormAI DATASET v1.0 Category: Wireless Network Scanner ; Style: surrealist
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>

#define PORT 8000
#define MAX_BUFFER_SIZE 1024

// Function declarations
void start_scan();
void *handle_connection(void *);
int is_valid_ip(char *);

// Global variables
int sock_fd;

int main(int argc, char *argv[]) {
    printf("Welcome to the Surrealistic Wireless Network Scanner!\n");

    // Create socket
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    int opt = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Error setting socket options");
        exit(EXIT_FAILURE);
    }

    // Bind socket
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Start scanning for connections
    start_scan();

    // Close socket
    close(sock_fd);

    return 0;
}

void start_scan() {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[MAX_BUFFER_SIZE] = {0};
    pthread_t thread_id;

    while(1) {
        // Accept incoming connection
        int client_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("Error accepting connection");
            continue;
        }

        // Handle connection on a separate thread
        if (pthread_create(&thread_id, NULL, handle_connection, (void *)&client_fd)) {
            perror("Error creating thread");
            close(client_fd);
            continue;
        }

        // Detach thread so it can run independently
        if (pthread_detach(thread_id)) {
            perror("Error detaching thread");
            close(client_fd);
            continue;
        }
    }
}

void *handle_connection(void *arg) {
    int client_fd = *((int *)arg);
    char buffer[MAX_BUFFER_SIZE] = {0};
    char reply[MAX_BUFFER_SIZE] = {0};

    // Get client IP address
    struct sockaddr_in client_addr = {0};
    socklen_t addr_len = sizeof(client_addr);
    if (getpeername(client_fd, (struct sockaddr *)&client_addr, &addr_len)) {
        perror("Error getting client IP address");
        close(client_fd);
        pthread_exit(NULL);
    }

    char *ip_addr = inet_ntoa(client_addr.sin_addr);

    printf("Scanning network from %s...\n", ip_addr);

    // Parse client IP address
    char prefix[16] = {0};
    strcpy(prefix, strtok(ip_addr, "."));
    for (int i = 0; i < 3; i++) {
        strcat(prefix, ".");
        strcat(prefix, strtok(NULL, "."));
    }
    strcat(prefix, ".");

    // Scan network for active IP addresses
    for (int i = 1; i <= 255; i++) {
        char test_ip[16] = {0};
        sprintf(test_ip, "%s%d", prefix, i);
        if (!is_valid_ip(test_ip)) {
            continue;
        }

        // Create test socket for checking if IP address is active
        int test_sock = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in test_addr = {0};
        test_addr.sin_family = AF_INET;
        test_addr.sin_port = htons(PORT);
        test_addr.sin_addr.s_addr = inet_addr(test_ip);
        int result = connect(test_sock, (struct sockaddr *)&test_addr, sizeof(test_addr));

        // Check if IP address is active
        if (result >= 0) {
            sprintf(reply, "Active IP address found: %s", test_ip);
            send(client_fd, reply, strlen(reply), 0);
            printf("%s\n", reply);
            close(test_sock);
        }
    }

    // Send end of transmission signal
    strcpy(reply, "EOT");
    send(client_fd, reply, strlen(reply), 0);

    // Close client socket and exit thread
    close(client_fd);
    pthread_exit(NULL);
}

int is_valid_ip(char *ip) {
    char *token = strtok(ip, ".");
    int octet_count = 0;

    while (token != NULL) {
        if (!isdigit(*token)) {
            return 0;
        }

        int octet = atoi(token);
        if (octet < 0 || octet > 255) {
            return 0;
        }

        token = strtok(NULL, ".");
        octet_count++;
    }

    if (octet_count != 4) {
        return 0;
    }

    return 1;
}