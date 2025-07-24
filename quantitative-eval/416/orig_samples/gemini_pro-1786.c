//GEMINI-pro DATASET v1.0 Category: Intrusion detection system ; Style: lively
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

// Global variables
int sockfd;
struct sockaddr_in serv_addr;

// Function to handle client connections
void *handle_client(void *arg) {
  int newsockfd = *(int *)arg;
  char buffer[256];

  // Receive data from the client
  int n = read(newsockfd, buffer, sizeof(buffer));
  if (n < 0) {
    perror("Error reading from socket");
    exit(1);
  }

  // Check for suspicious activity
  if (strstr(buffer, "DROP") || strstr(buffer, "DELETE") || strstr(buffer, "TRUNCATE")) {
    // Log the suspicious activity
    printf("Suspicious activity detected: %s\n", buffer);

    // Send an alert to the administrator
    char message[256];
    sprintf(message, "Suspicious activity detected: %s", buffer);
    send(sockfd, message, strlen(message), 0);
  }

  // Close the client connection
  close(newsockfd);

  return NULL;
}

int main() {
  // Create a socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("Error creating socket");
    exit(1);
  }

  // Set up the server address
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(5000);
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  // Bind the socket to the server address
  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Error binding socket");
    exit(1);
  }

  // Listen for incoming connections
  if (listen(sockfd, 5) < 0) {
    perror("Error listening on socket");
    exit(1);
  }

  // Main loop
  while (1) {
    // Accept a new connection
    int newsockfd = accept(sockfd, (struct sockaddr *)NULL, NULL);
    if (newsockfd < 0) {
      perror("Error accepting connection");
      continue;
    }

    // Create a new thread to handle the client connection
    pthread_t tid;
    pthread_create(&tid, NULL, handle_client, (void *)&newsockfd);
  }

  // Close the server socket
  close(sockfd);

  return 0;
}