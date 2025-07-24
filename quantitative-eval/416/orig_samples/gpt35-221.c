//FormAI DATASET v1.0 Category: Client Server Application ; Style: all-encompassing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 8080

void *handle_client(void *);

int main(int argc, char const *argv[]) {
  int server_fd, new_socket;
  struct sockaddr_in server_addr;
  pthread_t thread;
  int opt = 1;
  int addrlen = sizeof(server_addr);

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Setting socket options
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  // Setting socket address
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  // Binding socket
  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // Listening for incoming connections
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  printf("Server started on port %d...\n", PORT);

  // Accepting new connections
  while ((new_socket = accept(server_fd, (struct sockaddr *)&server_addr, (socklen_t *)&addrlen))) {
    printf("New connection accepted.\n");

    if (pthread_create(&thread, NULL, handle_client, (void *)&new_socket) < 0) {
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
  }

  if (new_socket < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  return 0;
}

void *handle_client(void *arg) {
  int sock = *(int *)arg;
  int read_size;
  char *message = "Hello, client!";

  // Sending welcome message to client
  if (send(sock, message, strlen(message), 0) < 0) {
    perror("send");
  }

  // Reading messages from client
  char client_message[1024];
  while ((read_size = recv(sock, client_message, 1024, 0)) > 0) {
    // Adding null terminator to client message
    client_message[read_size] = '\0';

    // Printing client message to console
    printf("Received message from client: %s\n", client_message);

    // Sending back message to client
    if (send(sock, client_message, strlen(client_message), 0) < 0) {
      perror("send");
      break;
    }

    // Clearing client_message buffer
    memset(client_message, 0, sizeof(client_message));
  }

  if (read_size == 0) {
    printf("Client disconnected.\n");
    fflush(stdout);
  } else if (read_size == -1) {
    perror("recv");
  }

  // Closing socket
  close(sock);

  return NULL;
}