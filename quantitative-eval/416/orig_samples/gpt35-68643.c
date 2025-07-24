//FormAI DATASET v1.0 Category: Socket programming ; Style: multiplayer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Define the maximum number of players
#define MAX_PLAYERS 4

// Define the size of the buffer used for sending/receiving messages
#define BUFFER_SIZE 1024

// Define the server port number
#define SERVER_PORT 12345

// Define the client structure
typedef struct {
    int socket;
    char name[BUFFER_SIZE];
} client_t;

// Define the players' list and the mutex to protect it
client_t players[MAX_PLAYERS];
pthread_mutex_t players_mutex = PTHREAD_MUTEX_INITIALIZER;

// Define the thread function to handle a client's connection
void *client_thread(void *arg) {
    // Extract the socket from the argument
    int client_socket = *(int*)arg;
    
    // Receive the client's name
    char name[BUFFER_SIZE];
    read(client_socket, name, BUFFER_SIZE);
    
    // Add the client to the players' list
    pthread_mutex_lock(&players_mutex);
    int player_index = -1;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (players[i].socket == 0) {
            players[i].socket = client_socket;
            strncpy(players[i].name, name, BUFFER_SIZE);
            player_index = i;
            break;
        }
    }
    pthread_mutex_unlock(&players_mutex);
    
    // Check if the maximum number of players has been reached
    if (player_index == -1) {
        char message[BUFFER_SIZE];
        snprintf(message, BUFFER_SIZE, "Sorry, the game is already full.");
        write(client_socket, message, strlen(message));
        close(client_socket);
        return NULL;
    }
    
    // Notify the client about the other players' names
    char message[BUFFER_SIZE];
    int players_count = 0;
    pthread_mutex_lock(&players_mutex);
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (players[i].socket != 0 && i != player_index) {
            snprintf(message, BUFFER_SIZE, "Player %s has joined the game.", name);
            write(players[i].socket, message, strlen(message));
            snprintf(message, BUFFER_SIZE, "%s", players[i].name);
            write(client_socket, message, strlen(message));
            players_count++;
        }
    }
    pthread_mutex_unlock(&players_mutex);
    
    // Wait for the game to start
    while (players_count != MAX_PLAYERS) {
      sleep(1);
    }
    
    // Start the game
    char game_message[BUFFER_SIZE];
    snprintf(game_message, BUFFER_SIZE, "The game has started!");
    for (int i = 0; i < MAX_PLAYERS; i++) {
        write(players[i].socket, game_message, strlen(game_message));
    }
    
    // Receive the client's messages until the game ends
    char buffer[BUFFER_SIZE];
    while (1) {
        int bytes = read(client_socket, buffer, BUFFER_SIZE);
        if (bytes <= 0) break;
        pthread_mutex_lock(&players_mutex);
        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (players[i].socket != 0 && i != player_index) {
                write(players[i].socket, buffer, bytes);
            }
        }
        pthread_mutex_unlock(&players_mutex);
    }
    
    // Remove the client from the players' list
    pthread_mutex_lock(&players_mutex);
    players[player_index].socket = 0;
    pthread_mutex_unlock(&players_mutex);
    
    // Notify the other players that the client has left
    pthread_mutex_lock(&players_mutex);
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (players[i].socket != 0 && i != player_index) {
            snprintf(message, BUFFER_SIZE, "Player %s has left the game.", name);
            write(players[i].socket, message, strlen(message));
        }
    }
    pthread_mutex_unlock(&players_mutex);
    
    // Close the client's socket
    close(client_socket);
    
    return NULL;
}

// Define the main function
int main() {
    // Initialize the players' list
    for (int i = 0; i < MAX_PLAYERS; i++) {
        players[i].socket = 0;
        memset(players[i].name, 0, BUFFER_SIZE);
    }
    
    // Create the server socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Failed to create server socket");
        exit(1);
    }
    
    // Bind the server socket to the port and start listening for connections
    struct sockaddr_in server_address = {0};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Failed to bind server socket");
        exit(1);
    }
    if (listen(server_socket, MAX_PLAYERS) == -1) {
        perror("Failed to start listening for connections");
        exit(1);
    }
    
    // Accept clients' connections in a loop
    while (1) {
        // Accept a client's connection
        struct sockaddr_in client_address = {0};
        socklen_t client_address_size = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_size);
        if (client_socket == -1) {
            perror("Failed to accept client connection");
            exit(1);
        }
        
        // Create a thread to handle the client's connection
        pthread_t thread;
        if (pthread_create(&thread, NULL, client_thread, &client_socket) != 0) {
            perror("Failed to create client thread");
            exit(1);
        }
        pthread_detach(thread);
    }
    
    // Close the server socket
    close(server_socket);
    
    return 0;
}