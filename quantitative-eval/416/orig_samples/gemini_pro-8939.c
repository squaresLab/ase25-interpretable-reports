//GEMINI-pro DATASET v1.0 Category: GPS Navigation Simulation ; Style: distributed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_PORT 5000
#define BUFFER_SIZE 1024

// Structure to store the GPS data
typedef struct {
    double latitude;
    double longitude;
    double altitude;
    double speed;
    double heading;
} GPSData;

// Function to handle client connections
void *handle_client(void *arg) {
    int client_sock = *(int *)arg;
    char buffer[BUFFER_SIZE];
    GPSData gps_data;

    // Receive the GPS data from the client
    recv(client_sock, &gps_data, sizeof(GPSData), 0);

    // Print the GPS data to the console
    printf("Received GPS data: latitude = %.6f, longitude = %.6f, altitude = %.2f, speed = %.2f, heading = %.2f\n",
           gps_data.latitude, gps_data.longitude, gps_data.altitude, gps_data.speed, gps_data.heading);

    // Send a confirmation message to the client
    strcpy(buffer, "GPS data received");
    send(client_sock, buffer, strlen(buffer), 0);

    // Close the client socket
    close(client_sock);
    return NULL;
}

int main() {
    // Create a server socket
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        perror("Error creating server socket");
        return EXIT_FAILURE;
    }

    // Bind the server socket to the specified port
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding server socket");
        close(server_sock);
        return EXIT_FAILURE;
    }

    // Listen for incoming client connections
    if (listen(server_sock, 5) == -1) {
        perror("Error listening for client connections");
        close(server_sock);
        return EXIT_FAILURE;
    }

    while (1) {
        // Accept an incoming client connection
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sock == -1) {
            perror("Error accepting client connection");
            continue;
        }

        // Create a new thread to handle the client connection
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, (void *)&client_sock) != 0) {
            perror("Error creating thread");
            close(client_sock);
            continue;
        }
    }

    // Close the server socket
    close(server_sock);
    return EXIT_SUCCESS;
}