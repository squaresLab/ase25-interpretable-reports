//FormAI DATASET v1.0 Category: Chat server ; Style: intelligent
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define max_clients 2 //maximum number of clients that the server can handle

int num_clients = 0;
int client_socket[max_clients];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //mutex lock for thread safety

void *client_communication(void *arg) {
  char buffer[1024];
  int client_num = *(int*)arg;
  int client_fd = client_socket[client_num];

  while(1) {
    memset(buffer, 0, sizeof(buffer));
    ssize_t count = recv(client_fd, buffer, sizeof(buffer), 0); //receive message from client
    if(count <= 0) {
      pthread_mutex_lock(&mutex);
      close(client_fd);
      printf("Client %d disconnected.\n", client_num+1);
      client_socket[client_num] = 0;
      num_clients--;
      pthread_mutex_unlock(&mutex);
      return NULL;
    }

    printf("Client %d: %s", client_num+1, buffer);

    pthread_mutex_lock(&mutex);
    for(int i=0; i<max_clients; i++) {
      if(client_socket[i] != 0 && i != client_num) { //send to all other clients
        send(client_socket[i], buffer, sizeof(buffer), 0);
      }
    }
    pthread_mutex_unlock(&mutex);
  }
}

int main() {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(server_fd < 0) {
    perror("socket() failed");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_address, client_address;
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(8080);

  int address_len = sizeof(server_address);

  if(bind(server_fd, (struct sockaddr *)&server_address, address_len) < 0) {
    perror("bind() failed");
    exit(EXIT_FAILURE);
  }

  if(listen(server_fd, 2) < 0) { //server can handle maximum 2 clients
    perror("listen() failed");
    exit(EXIT_FAILURE);
  }

  printf("Server is listening on port 8080...\n");

  while(num_clients < max_clients) { //accept clients and create threads for them
    int client_fd = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t *)&address_len);

    if(client_fd < 0) {
      perror("accept() failed");
      exit(EXIT_FAILURE);
    }

    printf("Connection accepted from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

    pthread_mutex_lock(&mutex);
    client_socket[num_clients] = client_fd;
    printf("Client %d connected.\n", num_clients+1);
    num_clients++;

    pthread_t thread;
    int arg = num_clients-1;
    pthread_create(&thread, NULL, client_communication, &arg);
    pthread_mutex_unlock(&mutex);
  }
  return 0;
}