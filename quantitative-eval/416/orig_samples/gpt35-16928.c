//FormAI DATASET v1.0 Category: Chat server ; Style: intelligent
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

// global variables
#define MAX_CLIENTS 10
int client_sockets[MAX_CLIENTS];
int num_clients = 0;

// function to handle client connections
void* handle_client(void* arg) {
  int client_socket = *(int*)arg;
  char buffer[256];

  while (1) {
    // read client message
    bzero(buffer, 256);
    int n = read(client_socket, buffer, 255);
    if (n < 0) {
      perror("Error reading from socket");
      break;
    }

    // broadcast message to all other clients
    for (int i = 0; i < num_clients; i++) {
      if (client_sockets[i] != client_socket) {
        n = write(client_sockets[i], buffer, strlen(buffer));
        if (n < 0) {
          perror("Error writing to socket");
          break;
        }
      }
    }
  }

  // remove client from list
  for (int i = 0; i < num_clients; i++) {
    if (client_sockets[i] == client_socket) {
      for (int j = i; j < num_clients - 1; j++) {
        client_sockets[j] = client_sockets[j + 1];
      }
      num_clients--;
      break;
    }
  }

  // close connection and exit thread
  close(client_socket);
  pthread_exit(NULL);
}

int main() {
  // create server socket
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    perror("Error creating socket");
    exit(1);
  }

  // set server address
  struct sockaddr_in server_address;
  bzero((char*) &server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(8080);

  // bind server socket to address
  if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
    perror("Error binding socket to address");
    exit(1);
  }

  // listen for client connections
  listen(server_socket, 5);
  printf("Server listening on port 8080...\n");

  while (1) {
    // accept client connection
    struct sockaddr_in client_address;
    socklen_t client_address_length = sizeof(client_address);
    int client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_address_length);
    if (client_socket < 0) {
      perror("Error accepting client connection");
      continue;
    }

    // limit number of clients
    if (num_clients >= MAX_CLIENTS) {
      printf("Maximum number of clients reached\n");
      close(client_socket);
      continue;
    }

    // add client to list
    client_sockets[num_clients] = client_socket;
    num_clients++;

    // create thread to handle client
    pthread_t thread_id;
    int result = pthread_create(&thread_id, NULL, handle_client, &client_socket);
    if (result != 0) {
      perror("Error creating thread");
      continue;
    }

    // detach thread
    pthread_detach(thread_id);
  }

  // close server socket
  close(server_socket);
  return 0;
}