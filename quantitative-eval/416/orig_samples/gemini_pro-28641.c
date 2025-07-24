//GEMINI-pro DATASET v1.0 Category: Client Server Application ; Style: innovative
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

// Client
void *client_thread_func(void *arg) {
    int sockfd = *((int *) arg);
    char buf[1024];
    while (1) {
        printf("Enter message: ");
        fgets(buf, sizeof(buf), stdin);
        send(sockfd, buf, strlen(buf), 0);
    }
    return NULL;
}

// Server
void handle_client(int sockfd) {
    char buf[1024];
    while (1) {
        int n = recv(sockfd, buf, sizeof(buf), 0);
        if (n == 0) {
            close(sockfd);
            return;
        } else if (n < 0) {
            if (errno == EINTR) {
                continue;
            }
            perror("Error receiving data");
            close(sockfd);
            return;
        }
        printf("Received message: %s", buf);
    }
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <server IP address> <port>\n", argv[0]);
        return 1;
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return 1;
    }
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        perror("Error converting IP address");
        close(sockfd);
        return 1;
    }
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("Error connecting to server");
        close(sockfd);
        return 1;
    }
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, client_thread_func, &sockfd);
    if (ret != 0) {
        perror("Error creating client thread");
        close(sockfd);
        return 1;
    }
    handle_client(sockfd);
    close(sockfd);
    return 0;
}