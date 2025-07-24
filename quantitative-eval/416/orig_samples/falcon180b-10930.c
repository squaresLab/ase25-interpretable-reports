//Falcon-180B DATASET v1.0 Category: Wi-Fi Signal Strength Analyzer ; Style: decentralized
// This program analyzes the Wi-Fi signal strength in a decentralized style

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define PORT 8080
#define SSID_MAX_LEN 32
#define PASSWORD_MAX_LEN 64

struct client {
    int sock;
    char ssid[SSID_MAX_LEN];
    char password[PASSWORD_MAX_LEN];
};

sem_t clients;
int num_clients = 0;
struct client clients_list[MAX_CLIENTS];

void *handle_client(void *arg) {
    int client_sock = *(int *) arg;
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_sock, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            break;
        }

        printf("Client %d sent: %s\n", client_sock, buffer);
    }

    close(client_sock);

    sem_post(&clients);
}

int main() {
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr));

    listen(server_sock, 3);

    sem_init(&clients, 0, MAX_CLIENTS);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        int client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &client_addr_size);

        if (num_clients >= MAX_CLIENTS) {
            close(client_sock);
            continue;
        }

        sem_wait(&clients);

        strcpy(clients_list[num_clients].ssid, "");
        strcpy(clients_list[num_clients].password, "");

        clients_list[num_clients].sock = client_sock;
        num_clients++;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client_sock);
    }

    close(server_sock);

    return 0;
}