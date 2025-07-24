//GEMINI-pro DATASET v1.0 Category: Smart home automation ; Style: imaginative
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>

// Define the smart home devices
#define LIGHT 1
#define FAN 2
#define DOOR 3
#define WINDOW 4

// Define the smart home device states
#define ON 1
#define OFF 0

// Define the smart home device names
char *device_names[] = {"Light", "Fan", "Door", "Window"};

// Define the smart home device states
int device_states[] = {OFF, OFF, OFF, OFF};

// Define the smart home device control commands
char *device_control_commands[] = {"on", "off"};

// Define the smart home device control function
void control_device(int device, int state) {
  device_states[device] = state;
  printf("%s is now %s\n", device_names[device], device_control_commands[state]);
}

// Define the smart home device status function
void get_device_status(int device) {
  printf("%s is %s\n", device_names[device], device_control_commands[device_states[device]]);
}

// Define the smart home server function
void *server_thread(void *args) {
  int sockfd = *(int *)args;
  char buffer[1024];
  while (1) {
    memset(buffer, 0, sizeof(buffer));
    int n = read(sockfd, buffer, sizeof(buffer));
    if (n <= 0) {
      perror("Error reading from socket");
      close(sockfd);
      pthread_exit(NULL);
    }
    char *command = strtok(buffer, " ");
    if (strcmp(command, "control") == 0) {
      char *device_name = strtok(NULL, " ");
      char *state = strtok(NULL, " ");
      int device = -1;
      for (int i = 0; i < 4; i++) {
        if (strcmp(device_name, device_names[i]) == 0) {
          device = i;
          break;
        }
      }
      if (device == -1) {
        printf("Invalid device name\n");
      } else {
        int state_int = -1;
        for (int i = 0; i < 2; i++) {
          if (strcmp(state, device_control_commands[i]) == 0) {
            state_int = i;
            break;
          }
        }
        if (state_int == -1) {
          printf("Invalid state\n");
        } else {
          control_device(device, state_int);
        }
      }
    } else if (strcmp(command, "status") == 0) {
      char *device_name = strtok(NULL, " ");
      int device = -1;
      for (int i = 0; i < 4; i++) {
        if (strcmp(device_name, device_names[i]) == 0) {
          device = i;
          break;
        }
      }
      if (device == -1) {
        printf("Invalid device name\n");
      } else {
        get_device_status(device);
      }
    } else {
      printf("Invalid command\n");
    }
  }
  return NULL;
}

int main() {
  // Create a socket for the server
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("Error creating socket");
    return EXIT_FAILURE;
  }

  // Bind the socket to the server address
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(5000);
  if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
    perror("Error binding socket");
    return EXIT_FAILURE;
  }

  // Listen for incoming connections
  if (listen(sockfd, 5) == -1) {
    perror("Error listening for connections");
    return EXIT_FAILURE;
  }

  // Create a thread for each incoming connection
  while (1) {
    struct sockaddr_in client_addr;
    int client_len = sizeof(client_addr);
    int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
    if (client_sockfd == -1) {
      perror("Error accepting connection");
      continue;
    }
    pthread_t thread;
    if (pthread_create(&thread, NULL, server_thread, &client_sockfd) != 0) {
      perror("Error creating thread");
      close(client_sockfd);
      continue;
    }
  }

  // Close the server socket
  close(sockfd);

  return EXIT_SUCCESS;
}