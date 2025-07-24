//GEMINI-pro DATASET v1.0 Category: Checksum Calculator ; Style: multiplayer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 5000
#define MAX_CLIENTS 10

int client_sockets[MAX_CLIENTS];
int client_count = 0;

void *handle_client(void *arg) {
  int client_socket = *(int *)arg;
  char buffer[1024];
  while (1) {
    memset(buffer, 0, sizeof(buffer));
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received <= 0) {
      printf("Client disconnected\n");
      close(client_socket);
      break;
    }
    int checksum = 0;
    for (int i = 0; i < bytes_received; i++) {
      checksum += buffer[i];
    }
    printf("Checksum: %d\n", checksum);
    send(client_socket, &checksum, sizeof(checksum), 0);
  }
  return NULL;
}

int main() {
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    perror("socket");
    exit(1);
  }
  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  server_address.sin_addr.s_addr = INADDR_ANY;
  if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
    perror("bind");
    exit(1);
  }
  if (listen(server_socket, MAX_CLIENTS) < 0) {
    perror("listen");
    exit(1);
  }
  while (1) {
    int client_socket = accept(server_socket, NULL, NULL);
    if (client_socket < 0) {
      perror("accept");
      continue;
    }
    if (client_count >= MAX_CLIENTS) {
      close(client_socket);
      continue;
    }
    client_sockets[client_count] = client_socket;
    client_count++;
    pthread_t thread;
    pthread_create(&thread, NULL, handle_client, &client_socket);
  }
  return 0;
}