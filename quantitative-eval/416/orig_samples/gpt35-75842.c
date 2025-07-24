//FormAI DATASET v1.0 Category: Educational ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080

void *handle_client(void *);

int main(int argc, char const *argv[]) {
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  pthread_t thread;

  // Create TCP socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Set socket options
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    perror("setsockopt failed");
    exit(EXIT_FAILURE);
  }

  // Bind socket to address and port
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // Listen for incoming connections
  if (listen(server_fd, 3) < 0) {
    perror("listen failed");
    exit(EXIT_FAILURE);
  }

  printf("Server running on port %d\n", PORT);

  // Accept incoming connections in a loop
  while ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen))) {
    printf("New client connected: %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

    // Spawn a new thread to handle the client
    if (pthread_create(&thread, NULL, handle_client, (void *)&new_socket) < 0) {
      perror("pthread_create failed");
      exit(EXIT_FAILURE);
    }

    pthread_detach(thread);
  }

  return 0;
}

void *handle_client(void *socket_ptr) {
  int socket = *(int *)socket_ptr;
  int recv_len;
  char buffer[1024] = {0};

  while ((recv_len = recv(socket, buffer, 1024, 0)) > 0) {
    // Echo back the message to the client
    send(socket, buffer, recv_len, 0);
  }

  printf("Client disconnected\n");

  close(socket);
  return NULL;
}