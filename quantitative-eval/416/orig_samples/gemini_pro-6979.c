//GEMINI-pro DATASET v1.0 Category: Spam Detection System ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

// Define the maximum number of concurrent connections
#define MAX_CONNECTIONS 10

// Define the maximum size of a message
#define MAX_MESSAGE_SIZE 1024

// Define the port number to listen on
#define PORT 5000

// Define the IP address of the server
#define SERVER_IP "127.0.0.1"

// Define the spam detection algorithm
int is_spam(char *message) {
  // Check for common spam keywords
  if (strstr(message, "free") || strstr(message, "money") || strstr(message, "win")) {
    return 1;
  }

  // Check for excessive punctuation
  int punctuation_count = 0;
  for (int i = 0; i < strlen(message); i++) {
    if (message[i] == '.' || message[i] == ',' || message[i] == '!' || message[i] == '?') {
      punctuation_count++;
    }
  }
  if (punctuation_count > 5) {
    return 1;
  }

  // Check for suspicious links
  if (strstr(message, "http://") || strstr(message, "https://")) {
    return 1;
  }

  // Otherwise, the message is not spam
  return 0;
}

// Define the thread function
void *thread_function(void *arg) {
  // Get the socket descriptor from the argument
  int sockfd = *(int *)arg;

  // Read the message from the client
  char message[MAX_MESSAGE_SIZE];
  int n = read(sockfd, message, MAX_MESSAGE_SIZE);
  if (n < 0) {
    perror("read");
    exit(1);
  }

  // Check if the message is spam
  int is_spam_result = is_spam(message);

  // Send the result back to the client
  n = write(sockfd, &is_spam_result, sizeof(is_spam_result));
  if (n < 0) {
    perror("write");
    exit(1);
  }

  // Close the socket
  close(sockfd);

  return NULL;
}

int main() {
  // Create a socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket");
    exit(1);
  }

  // Set the socket options
  int optval = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
    perror("setsockopt");
    exit(1);
  }

  // Bind the socket to the port
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
  serv_addr.sin_port = htons(PORT);
  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("bind");
    exit(1);
  }

  // Listen for connections
  if (listen(sockfd, MAX_CONNECTIONS) < 0) {
    perror("listen");
    exit(1);
  }

  // Accept connections and spawn threads
  while (1) {
    // Accept a connection
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_sockfd < 0) {
      perror("accept");
      continue;
    }

    // Spawn a thread to handle the connection
    pthread_t thread;
    if (pthread_create(&thread, NULL, thread_function, &client_sockfd) != 0) {
      perror("pthread_create");
      continue;
    }
  }

  // Close the server socket
  close(sockfd);

  return 0;
}