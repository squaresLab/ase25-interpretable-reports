//Falcon-180B DATASET v1.0 Category: Network Topology Mapper ; Style: rigorous
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct {
    int sock;
    struct sockaddr_in addr;
    char name[20];
} client_t;

client_t clients[MAX_CLIENTS];
int num_clients = 0;

void *handle_client(void *arg) {
    int sock = *(int *) arg;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        printf("%s: %s\n", clients[num_clients].name, buffer);
    }

    close(sock);
    return NULL;
}

int main() {
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (server_sock == -1) {
        printf("Error creating socket\n");
        return 1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        printf("Error binding socket\n");
        return 1;
    }

    if (listen(server_sock, 10) == -1) {
        printf("Error listening on socket\n");
        return 1;
    }

    printf("Server listening on port 8080\n");

    while (1) {
        client_t new_client;
        socklen_t addr_size = sizeof(new_client.addr);
        new_client.sock = accept(server_sock, (struct sockaddr *) &new_client.addr, &addr_size);

        if (new_client.sock == -1) {
            printf("Error accepting connection\n");
            continue;
        }

        getpeername(new_client.sock, (struct sockaddr *) &new_client.addr, &addr_size);
        inet_ntop(AF_INET, &new_client.addr.sin_addr, new_client.name, 20);

        if (num_clients >= MAX_CLIENTS) {
            printf("Maximum number of clients reached\n");
            close(new_client.sock);
            continue;
        }

        clients[num_clients++] = new_client;
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &new_client.sock);
    }

    return 0;
}