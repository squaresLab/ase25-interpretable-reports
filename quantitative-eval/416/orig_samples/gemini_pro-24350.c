//GEMINI-pro DATASET v1.0 Category: Building a IMAP Client ; Style: multiplayer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 143
#define MAX_CLIENTS 10

int sockfd;
struct sockaddr_in serv_addr;
char buf[1024];

void *client_thread(void *arg) {
    int clientfd = *(int *)arg;
    while (1) {
        memset(buf, 0, sizeof(buf));
        if (recv(clientfd, buf, sizeof(buf), 0) == -1) {
            perror("recv");
            exit(EXIT_FAILURE);
        }
        printf("%s\n", buf);
        if (strcmp(buf, "QUIT") == 0) {
            close(clientfd);
            break;
        }
        memset(buf, 0, sizeof(buf));
        strcpy(buf, "Hello from server");
        if (send(clientfd, buf, sizeof(buf), 0) == -1) {
            perror("send");
            exit(EXIT_FAILURE);
        }
    }
    return NULL;
}

int main() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    if (listen(sockfd, MAX_CLIENTS) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    pthread_t threads[MAX_CLIENTS];
    int clientfds[MAX_CLIENTS];
    int i = 0;
    while (1) {
        int clientfd = accept(sockfd, NULL, NULL);
        if (clientfd == -1) {
            perror("accept");
            continue;
        }
        clientfds[i++] = clientfd;
        if (pthread_create(&threads[i], NULL, client_thread, (void *)&clientfd) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }
    for (i = 0; i < MAX_CLIENTS; i++) {
        pthread_join(threads[i], NULL);
    }
    close(sockfd);
    return 0;
}