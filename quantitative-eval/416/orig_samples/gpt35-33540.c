//FormAI DATASET v1.0 Category: Client Server Application ; Style: lively
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

void *handle_client(void *sock_fd) {
  int client_fd = *(int *)sock_fd;
  printf("Client %d connected\n", client_fd);

  char buffer[1024] = {0};
  while(1) {
    int valread = read(client_fd, buffer, 1024);
    if(valread == 0) {
      printf("Client %d disconnected\n", client_fd);
      close(client_fd);
      break;
    }
    printf("Received message from client %d: %s", client_fd, buffer);
    send(client_fd, buffer, strlen(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
  }
}

int main() {
  int server_fd;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);

  if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }
  if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    perror("setsockopt failed");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
  if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  if(listen(server_fd, 5) < 0) {
    perror("listen failed");
    exit(EXIT_FAILURE);
  }

  printf("Server is up and running!\n");

  while(1) {
    int client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if(client_fd < 0) {
      perror("accept failed");
      exit(EXIT_FAILURE);
    }
    pthread_t thread_id;
    if(pthread_create(&thread_id, NULL, handle_client, (void *)&client_fd) < 0) {
      perror("pthread_create failed");
      exit(EXIT_FAILURE);
    }
    pthread_detach(thread_id);
  }

  return 0;
}