//FormAI DATASET v1.0 Category: Chat server ; Style: protected
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENTS 10
#define BUF_SIZE 256

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int client_count = 0;
int client_sockets[MAX_CLIENTS];
char client_names[MAX_CLIENTS][BUF_SIZE];

void send_all(char* message, int sender_id);

void* client_handler(void* arg) {
  int client_id = *(int*) arg;
  int client_socket = client_sockets[client_id];
  char client_name[BUF_SIZE] = "";
  char buf[BUF_SIZE] = "";

  // Get client name
  strcpy(buf, "Enter your name: ");
  send(client_socket, buf, strlen(buf), 0);
  recv(client_socket, client_name, BUF_SIZE, 0);
  strcpy(client_names[client_id], client_name);

  while (1) {
    // Receive message
    memset(buf, 0, BUF_SIZE);
    if (recv(client_socket, buf, BUF_SIZE, 0) == 0) {
      printf("Client %d (%s) disconnected.\n", client_id, client_name);
      pthread_mutex_lock(&mutex);
      client_count--;
      client_sockets[client_id] = 0;
      strcpy(client_names[client_id], "");
      send_all("", client_id); // Notify other clients of disconnection
      pthread_mutex_unlock(&mutex);
      break;
    }

    // Handle command message
    if (strlen(buf) > 1 && buf[0] == '/') {
      if (strcmp(buf, "/list") == 0) {
        // Send list of connected clients
        char message[BUF_SIZE] = "Connected clients:\n";
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < MAX_CLIENTS; i++) {
          if (client_sockets[i] > 0) {
            strcat(message, client_names[i]);
            strcat(message, "\n");
          }
        }
        pthread_mutex_unlock(&mutex);
        send(client_socket, message, strlen(message), 0);
      } else {
        printf("Invalid command from client %d (%s): %s\n",
               client_id, client_name, buf);
      }
      continue;
    }

    // Send message to all clients
    char message[BUF_SIZE] = "";
    pthread_mutex_lock(&mutex);
    sprintf(message, "%s: %s", client_name, buf);
    send_all(message, client_id);
    pthread_mutex_unlock(&mutex);
  }

  close(client_socket);
  return NULL;
}

void send_all(char* message, int sender_id) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    int client_socket = client_sockets[i];
    if (client_socket > 0 && i != sender_id) {
      send(client_socket, message, strlen(message), 0);
    }
  }
}

int main() {
  int server_socket;
  struct sockaddr_in server_address;

  // Create socket
  if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    printf("Failed to create socket.\n");
    exit(1);
  }

  // Configure server address
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(5000);

  // Bind socket to address
  if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
    printf("Failed to bind socket.\n");
    close(server_socket);
    exit(1);
  }

  // Listen for connections
  if (listen(server_socket, MAX_CLIENTS) == -1) {
    printf("Failed to listen for connections.\n");
    close(server_socket);
    exit(1);
  }

  printf("Chat server started.\n");

  while (1) {
    // Accept incoming connection
    struct sockaddr_in client_address;
    memset(&client_address, 0, sizeof(client_address));
    socklen_t addr_size = sizeof(client_address);
    int client_socket = accept(server_socket, (struct sockaddr*) &client_address, &addr_size);
    if (client_socket == -1) {
      printf("Failed to accept connection.\n");
      continue;
    }

    // Handle connection in new thread
    pthread_t thread_id;
    int client_id = -1;
    pthread_mutex_lock(&mutex);
    if (client_count < MAX_CLIENTS) {
      // Find first available client slot
      for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] == 0) {
          client_id = i;
          client_sockets[client_id] = client_socket;
          client_count++;
          break;
        }
      }
    }
    pthread_mutex_unlock(&mutex);
    if (client_id == -1) {
      printf("Max clients reached. Connection closed.\n");
      close(client_socket);
      continue;
    }
    if (pthread_create(&thread_id, NULL, client_handler, (void*) &client_id) != 0) {
      printf("Failed to create client thread.\n");
      close(client_socket);
      continue;
    }
  }

  close(server_socket);
  return 0;
}