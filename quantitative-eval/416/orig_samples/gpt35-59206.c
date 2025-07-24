//FormAI DATASET v1.0 Category: Networking ; Style: immersive
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 8080

void *client_handler(void *socket_desc) {
  int sock = *(int*)socket_desc;
  char buffer[1024] = {0};

  // Receive message from client
  int valread = read(sock, buffer, 1024);
  if(valread > 0) {
    printf("Client message: %s", buffer);
  }
  else {
    printf("Receive failed.\n");
  }

  // Send message to client
  char *message = "Server received message.\n";
  send(sock, message, strlen(message), 0);

  close(sock);
}

int main(int argc, char const *argv[]) {
  int server_fd, new_socket, opt = 1;
  struct sockaddr_in address;
  int addrlen = sizeof(address);

  // Creating socket
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(server_fd == 0) {
    perror("Failed to create socket.\n");
    exit(EXIT_FAILURE);
  }

  // Set socket options
  if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    perror("Failed to set socket options.\n");
    exit(EXIT_FAILURE);
  }

  // Set address information
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Bind socket to address
  if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("Failed to bind socket to address.\n");
    exit(EXIT_FAILURE);
  }

  // Listen for connections
  if(listen(server_fd, 3) < 0) {
    perror("Failed to listen for connections.\n");
    exit(EXIT_FAILURE);
  }

  printf("Listening...\n");

  // Accept connections and handle in separate threads
  while((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))) {
    pthread_t thread_id;
    if(pthread_create(&thread_id, NULL, client_handler, (void*)&new_socket) < 0) {
      perror("Failed to create thread.\n");
      exit(EXIT_FAILURE);
    }

    printf("Connection of Client %s:%d accepted.\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
  }

  if(new_socket < 0) {
    perror("Failed to accept connection.\n");
    exit(EXIT_FAILURE);
  }

  return 0;
}