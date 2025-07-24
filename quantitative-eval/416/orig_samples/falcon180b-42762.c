//Falcon-180B DATASET v1.0 Category: Smart home light control ; Style: Claude Shannon
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFER_SIZE 1024
#define PORT 8080
#define TRUE 1
#define FALSE 0

int sock;
struct sockaddr_in server_addr;
pthread_t thread_id;

void *handle_client(void *arg) {
    int client_socket = *(int *) arg;
    char buffer[BUFFER_SIZE];

    while (TRUE) {
        memset(buffer, 0, BUFFER_SIZE);
        recv(client_socket, buffer, BUFFER_SIZE, 0);

        if (strcmp(buffer, "on") == 0) {
            system("gpio write 18 1");
        } else if (strcmp(buffer, "off") == 0) {
            system("gpio write 18 0");
        } else if (strcmp(buffer, "status") == 0) {
            char response[20];
            sprintf(response, "Light is %s", (system("gpio read 18") == 0)? "off" : "on");
            send(client_socket, response, strlen(response), 0);
        }
    }

    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    int opt = TRUE;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
    listen(sock, 5);

    while (TRUE) {
        int client_socket = accept(sock, NULL, NULL);

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *) &client_socket);
    }

    close(sock);
    return 0;
}