//GEMINI-pro DATASET v1.0 Category: Drone Remote Control ; Style: expert-level
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>

#define SERVER_IP "192.168.1.1"
#define SERVER_PORT 8080
#define BUFF_SIZE 1024
#define MAX_CLIENTS 10

struct client_info {
    int socket;
    struct sockaddr_in addr;
    char name[32];
};

struct drone_info {
    int x;
    int y;
    int z;
    int yaw;
    int pitch;
    int roll;
};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct client_info clients[MAX_CLIENTS];
struct drone_info drone_info;
int num_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    char buff[BUFF_SIZE];
    int n;

    while (1) {
        memset(buff, 0, BUFF_SIZE);
        n = recv(client_socket, buff, BUFF_SIZE, 0);
        if (n <= 0) {
            break;
        }

        pthread_mutex_lock(&mutex);
        if (strncmp(buff, "name", 4) == 0) {
            strcpy(clients[num_clients].name, buff + 5);
            num_clients++;
        } else if (strncmp(buff, "move", 4) == 0) {
            sscanf(buff + 5, "%d %d %d %d %d %d", &drone_info.x, &drone_info.y, &drone_info.z, &drone_info.yaw, &drone_info.pitch, &drone_info.roll);
        }
        pthread_mutex_unlock(&mutex);
    }

    close(client_socket);
    return NULL;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    pthread_t client_thread;

    // Create the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        return -1;
    }

    // Set the server socket options
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
        perror("setsockopt");
        return -1;
    }

    // Bind the server socket to the IP address and port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        return -1;
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("listen");
        return -1;
    }

    // Accept incoming connections and create a thread for each client
    while (1) {
        client_addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("accept");
            return -1;
        }

        pthread_create(&client_thread, NULL, handle_client, &client_socket);
    }

    // Close the server socket
    close(server_socket);

    return 0;
}