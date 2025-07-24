//GEMINI-pro DATASET v1.0 Category: Socket programming ; Style: immersive
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

// A simple Socket programming example in C

// Define the port number for the server
#define PORT 8080

// Create a socket
int create_socket() {
  int sockfd;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("Error creating socket");
    exit(EXIT_FAILURE);
  }
  return sockfd;
}

// Bind the socket to a port
void bind_socket(int sockfd, int port) {
  struct sockaddr_in servaddr;
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);
  if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    perror("Error binding socket");
    exit(EXIT_FAILURE);
  }
}

// Listen for incoming connections
void listen_for_connections(int sockfd) {
  if (listen(sockfd, 5) < 0) {
    perror("Error listening for connections");
    exit(EXIT_FAILURE);
  }
}

// Accept an incoming connection
int accept_connection(int sockfd) {
  struct sockaddr_in cliaddr;
  int clilen = sizeof(cliaddr);
  int newsockfd;
  newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
  if (newsockfd < 0) {
    perror("Error accepting connection");
    exit(EXIT_FAILURE);
  }
  return newsockfd;
}

// Send data to a client
void send_data(int sockfd, char *data) {
  if (send(sockfd, data, strlen(data), 0) < 0) {
    perror("Error sending data");
    exit(EXIT_FAILURE);
  }
}

// Receive data from a client
char *receive_data(int sockfd) {
  char buffer[256];
  if (recv(sockfd, buffer, sizeof(buffer), 0) < 0) {
    perror("Error receiving data");
    exit(EXIT_FAILURE);
  }
  return buffer;
}

// Close a socket
void close_socket(int sockfd) {
  close(sockfd);
}

// The main function
int main() {
  int sockfd, newsockfd;
  char buffer[256];

  // Create a socket
  sockfd = create_socket();

  // Bind the socket to a port
  bind_socket(sockfd, PORT);

  // Listen for incoming connections
  listen_for_connections(sockfd);

  // Accept an incoming connection
  newsockfd = accept_connection(sockfd);

  // Receive data from the client
  strcpy(buffer, receive_data(newsockfd));

  // Print the data received from the client
  printf("Received data from the client: %s\n", buffer);

  // Send data to the client
  strcpy(buffer, "Hello from the server!");
  send_data(newsockfd, buffer);

  // Close the sockets
  close_socket(newsockfd);
  close_socket(sockfd);

  return 0;
}