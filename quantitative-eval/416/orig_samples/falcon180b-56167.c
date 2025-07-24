//Falcon-180B DATASET v1.0 Category: Smart home light control ; Style: optimized
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define PORT 8080

int clients[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[1024];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        recv(client_socket, buffer, sizeof(buffer), 0);
        printf("Client %d sent: %s\n", client_socket, buffer);

        if (strcmp(buffer, "light on") == 0) {
            system("echo 1 > /sys/class/leds/led1/brightness");
        } else if (strcmp(buffer, "light off") == 0) {
            system("echo 0 > /sys/class/leds/led1/brightness");
        }
    }
}

int main() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(server_socket, 5);

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        printf("Client connected on socket %d\n", client_socket);

        if (num_clients == MAX_CLIENTS) {
            send(client_socket, "Server is full\n", sizeof("Server is full\n"), 0);
            close(client_socket);
        } else {
            clients[num_clients++] = client_socket;
            pthread_t thread_id;
            pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
        }
    }

    return 0;
}