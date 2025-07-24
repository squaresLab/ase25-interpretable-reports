//Falcon-180B DATASET v1.0 Category: Smart home automation ; Style: intelligent
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define MAX_COMMAND_LENGTH 100

int clients[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[MAX_COMMAND_LENGTH];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, MAX_COMMAND_LENGTH, 0);

        if (bytes_received <= 0) {
            break;
        }

        char *command = strtok(buffer, " ");
        if (command == NULL) {
            continue;
        }

        if (strcmp(command, "ON") == 0) {
            system("echo 1 > /sys/class/leds/led0/brightness");
        } else if (strcmp(command, "OFF") == 0) {
            system("echo 0 > /sys/class/leds/led0/brightness");
        } else if (strcmp(command, "DIM") == 0) {
            system("echo 50 > /sys/class/leds/led0/brightness");
        } else if (strcmp(command, "QUIT") == 0) {
            close(client_socket);
            break;
        }
    }

    close(client_socket);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);

    printf("Smart Home Server is Running!\n");

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);

        if (num_clients >= MAX_CLIENTS) {
            close(client_socket);
            continue;
        }

        clients[num_clients++] = client_socket;
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
    }

    return 0;
}