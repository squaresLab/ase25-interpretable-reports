//FormAI DATASET v1.0 Category: Chat server ; Style: detailed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080

void *connection_handler(void *);

int main(int argc, char **argv) {

  int server_fd, new_socket, c;
  struct sockaddr_in server, client;

  // Create socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("Socket Creation Failed");
    exit(EXIT_FAILURE);
  }

  // Assign IP, PORT
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT);

  // Bind the socket with the server address
  if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("Bind Failed");
    exit(EXIT_FAILURE);
  }

  // Listen for incoming connections
  if (listen(server_fd, 3) < 0) {
    perror("Listen Failed");
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d\n", PORT);

  c = sizeof(struct sockaddr_in);
  while ((new_socket = accept(server_fd, (struct sockaddr *)&client, (socklen_t *)&c))) {
    printf("Connection accepted from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, connection_handler, (void *)&new_socket) < 0) {
      perror("Thread Creation Failed");
      return 1;
    }
  }

  if (new_socket < 0) {
    perror("Accept Failed");
    exit(EXIT_FAILURE);
  }

  close(server_fd);
  return 0;
}

void *connection_handler(void *socket_ptr) {
  int socket = *(int *)socket_ptr;
  char buffer[1024];
  int msg_len;

  // Send welcome message to connected client
  char *welcome_msg = "Welcome to the chat server! Type 'exit' to disconnect.";
  write(socket, welcome_msg, strlen(welcome_msg));

  while ((msg_len = read(socket, buffer, 1024)) > 0) {
    buffer[msg_len - 1] = '\0';   // Remove '\n' at the end of the message
    if (strcmp(buffer, "exit") == 0)
      break;   // Disconnect client if 'exit' is typed

    // Broadcast message to all connected clients
    printf("[%d] %s\n", socket, buffer);
    for (int i = 4; i < 1024; i++) {
      if (i == socket)
        continue;
      if (write(i, buffer, strlen(buffer)) < 0) {
        perror("Write Failed");
        break;
      }
    }
  }

  // Disconnect client
  printf("Connection closed from %d\n", socket);
  close(socket);
  return NULL;
}