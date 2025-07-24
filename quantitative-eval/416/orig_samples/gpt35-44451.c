//FormAI DATASET v1.0 Category: Chat server ; Style: all-encompassing
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENTS 10
// maximum number of clients that can connect to the server

int num_clients = 0; // number of active clients
int client_sockets[MAX_CLIENTS]; // array to store client sockets

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // mutex for thread safety

void *handle_client(void *socket_desc) {
  int sock = *(int*)socket_desc;
  char buff[1024];
  while(1) {
    memset(buff, 0, 1024);
    if(read(sock, buff, 1024) == 0) {
      // client disconnected
      printf("Client disconnected.\n");
      pthread_mutex_lock(&mutex);
      for(int i=0; i<num_clients; i++) {
        if(client_sockets[i] == sock) {
          close(client_sockets[i]);
          client_sockets[i] = client_sockets[num_clients-1];
          num_clients--;
          break;
        }
      }
      pthread_mutex_unlock(&mutex);
      break;
    }
    printf("Message: %s\n", buff);
    pthread_mutex_lock(&mutex);
    // send the message received to all clients
    for(int i=0; i<num_clients; i++) {
      if(client_sockets[i] != sock) {
        write(client_sockets[i], buff, strlen(buff));
      }
    }
    pthread_mutex_unlock(&mutex);
  }
  return 0;
}

int main(int argc, char *argv[]) {
  int server_sock, client_sock;
  struct sockaddr_in server_addr, client_addr;

  // create TCP socket
  server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(server_sock == -1) {
    perror("Error creating socket");
    exit(EXIT_FAILURE);
  }

  // set server address
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(9000);

  // bind socket to server address
  if(bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
    perror("Error binding socket");
    exit(EXIT_FAILURE);
  }

  // listen for incoming connections
  if(listen(server_sock, MAX_CLIENTS) == -1) {
    perror("Error listening for connections");
    exit(EXIT_FAILURE);
  }

  // accept incoming connections
  int client_addr_len = sizeof(client_addr);
  while((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, (socklen_t*)&client_addr_len))) {
    pthread_t thread_id;
    // handle at most MAX_CLIENTS at once
    if(num_clients < MAX_CLIENTS) {
      client_sockets[num_clients] = client_sock;
      num_clients++;
      // create a new thread to handle the client
      if(pthread_create(&thread_id, NULL, handle_client, (void*)&client_sock) < 0) {
        perror("Error creating thread");
        exit(EXIT_FAILURE);
      }
      printf("Accepted new connection from %s:%d (%d/%d clients)\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), num_clients, MAX_CLIENTS);
      // send welcome message to client
      char welcome_msg[] = "Welcome to the chat server! Please type your message and press enter.\n";
      write(client_sock, welcome_msg, strlen(welcome_msg));
    }
    else {
      char too_many_clients[] = "The server is full. Please try again later.\n";
      write(client_sock, too_many_clients, strlen(too_many_clients));
      close(client_sock);
    }
  }

  if(client_sock == -1) {
    perror("Error accepting connection");
    exit(EXIT_FAILURE);
  }

  return 0;
}