//GEMINI-pro DATASET v1.0 Category: Intrusion detection system ; Style: recursive
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CONNECTIONS 100
#define MAX_BUFFER_SIZE 1024
#define MAX_RULES 100

typedef struct {
    char *pattern;
    int pattern_length;
    int threshold;
} rule;

typedef struct {
    int fd;
    struct sockaddr_in addr;
    int rule_index;
    int matches;
} connection;

rule rules[MAX_RULES];
int num_rules;
connection connections[MAX_CONNECTIONS];
int num_connections;

void *handle_connection(void *arg) {
    connection *conn = (connection *)arg;

    char buffer[MAX_BUFFER_SIZE];

    while (1) {
        int bytes_read = recv(conn->fd, buffer, MAX_BUFFER_SIZE, 0);

        if (bytes_read <= 0) {
            break;
        }

        for (int i = 0; i < num_rules; i++) {
            if (strstr(buffer, rules[i].pattern) != NULL) {
                conn->matches++;

                if (conn->matches >= rules[i].threshold) {
                    printf("Intrusion detected from %s:%d!\n", inet_ntoa(conn->addr.sin_addr), ntohs(conn->addr.sin_port));
                    close(conn->fd);
                    pthread_exit(NULL);
                }
            }
        }
    }

    pthread_exit(NULL);
}

int main() {
    int listenfd;
    struct sockaddr_in servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if (listenfd < 0) {
        perror("socket");
        exit(1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(1337);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind");
        exit(1);
    }

    if (listen(listenfd, MAX_CONNECTIONS) < 0) {
        perror("listen");
        exit(1);
    }

    while (1) {
        struct sockaddr_in cliaddr;
        socklen_t clilen = sizeof(cliaddr);

        int connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

        if (connfd < 0) {
            perror("accept");
            continue;
        }

        if (num_connections >= MAX_CONNECTIONS) {
            printf("Maximum number of connections reached!\n");
            close(connfd);
            continue;
        }

        connection conn = {connfd, cliaddr, -1, 0};
        connections[num_connections++] = conn;

        pthread_t thread;
        pthread_create(&thread, NULL, handle_connection, (void *)&conn);
    }

    return 0;
}