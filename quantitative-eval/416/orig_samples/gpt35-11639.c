//FormAI DATASET v1.0 Category: Socket programming ; Style: multiplayer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080

typedef struct {
  int socket_fd;
  char username[20];
} client_data;

client_data clients[100];
int num_clients = 0;

void *handle_client(void *cli_socket) {
  int socket_fd = *(int *)cli_socket;
  char client_username[20];
  read(socket_fd, client_username, sizeof(client_username));
  
  // store client data
  client_data new_client = { socket_fd };
  strcpy(new_client.username, client_username);
  clients[num_clients++] = new_client; 
  
  char msg[1024];
  while(1) {
    // receive message from client
    char buffer[1024] = {0};
    int valread = read(socket_fd, buffer, 1024); 
    if (valread == 0) {
      // client has disconnected
      close(socket_fd);
      printf("%s has disconnected\n", client_username);
      pthread_exit(NULL);
    }
    
    // format client message
    memset(msg, 0, sizeof(msg));
    sprintf(msg, "%s: %s", client_username, buffer);
    
    // broadcast message to all clients
    for (int i = 0; i < num_clients; i++) {
      if (clients[i].socket_fd != socket_fd) {
        send(clients[i].socket_fd, msg, strlen(msg), 0);
      }
    }
  }
}

int main(int argc, char const *argv[]) {
  int server_socket_fd;
  struct sockaddr_in address;
  
  // setup server socket
  if ((server_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
    perror("socket failed"); 
    exit(EXIT_FAILURE); 
  }
  int opt = 1;
  if (setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) { 
    perror("setsockopt"); 
    exit(EXIT_FAILURE); 
  }
  address.sin_family = AF_INET; 
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( PORT );
  if (bind(server_socket_fd, (struct sockaddr *)&address, sizeof(address))<0) { 
    perror("bind failed"); 
    exit(EXIT_FAILURE); 
  }
  if (listen(server_socket_fd, 3) < 0) { 
    perror("listen"); 
    exit(EXIT_FAILURE); 
  }
  
  // main server loop
  while(1) {
    int new_client_socket;
    struct sockaddr_in client_addr;
    int addrlen = sizeof(client_addr);
    if ((new_client_socket = accept(server_socket_fd, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen))<0) {
        perror("accept"); 
        exit(EXIT_FAILURE); 
    }
    
    // create thread to handle client
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, &handle_client, (void *)&new_client_socket)) {
      perror("pthread_create");
    }
  }
  
  return 0;
}