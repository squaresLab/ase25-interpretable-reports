//FormAI DATASET v1.0 Category: Simple HTTP Proxy ; Style: surprised
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define BACKLOG 10
#define MAXLINE 1024
#define PORT 8888

void *handle_connection(void *fd) {
  int client_fd = *(int*)fd;
  char buffer[MAXLINE];
  int n;
  while ((n = recv(client_fd, buffer, MAXLINE, 0)) > 0) {
    printf("Received %d bytes\n", n);
    fwrite(buffer, sizeof(char), n, stdout);
    send(client_fd, buffer, n, 0);
  }
  close(client_fd);
  printf("Connection closed\n");
  return NULL;
}

int main() {
  printf("Surprise! Here's a simple HTTP proxy example in C!\n");
  printf("--------------------------------------------------\n\n");
  
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_addr = {
    .sin_family = AF_INET,
    .sin_port = htons(PORT),
    .sin_addr.s_addr = htonl(INADDR_ANY)
  };

  int optval = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, BACKLOG) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  printf("Listening on port %d...\n", PORT);
  while (1) {
    struct sockaddr_in client_addr;
    socklen_t client_size = sizeof(struct sockaddr_in);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_size);
    if (client_fd < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }
    printf("Incoming HTTP request from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    pthread_t tid;
    pthread_create(&tid, NULL, handle_connection, &client_fd);
  }
  return EXIT_SUCCESS;
}