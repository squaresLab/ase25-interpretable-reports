//Falcon-180B DATASET v1.0 Category: Smart home light control ; Style: Ken Thompson
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define PORT 5000

int sock;
int client_socks[MAX_CLIENTS];
int num_clients = 0;
int quit = 0;

void *handle_client(void *arg) {
    int client_sock = *(int *) arg;

    char buffer[1024];
    while (1) {
        bzero(buffer, 1024);
        int bytes_received = recv(client_sock, buffer, 1024, 0);

        if (bytes_received <= 0) {
            break;
        }

        if (strstr(buffer, "LIGHT ON")!= NULL) {
            system("gpio -g write 4 1");
        } else if (strstr(buffer, "LIGHT OFF")!= NULL) {
            system("gpio -g write 4 0");
        }
    }

    close(client_sock);

    return NULL;
}

void accept_connections() {
    while (!quit) {
        socklen_t addr_size;
        struct sockaddr_in client_addr;
        addr_size = sizeof(client_addr);

        int client_sock = accept(sock, (struct sockaddr *) &client_addr, &addr_size);

        if (num_clients >= MAX_CLIENTS) {
            send(client_sock, "Server is full. Please try again later.\r\n", 51, 0);
            close(client_sock);
            continue;
        }

        client_socks[num_clients] = client_sock;
        num_clients++;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client_sock);
    }
}

int main() {
    sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(sock, 10);

    accept_connections();

    return 0;
}