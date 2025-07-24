//GEMINI-pro DATASET v1.0 Category: Network Ping Test ; Style: multiplayer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

// Server address and port
#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8080

// Maximum number of players
#define MAX_PLAYERS 4

// Player struct
typedef struct {
    int id;
    char name[20];
    int score;
    int ping;
} player;

// Player array
player players[MAX_PLAYERS];

// Mutex for player array
pthread_mutex_t players_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to send a ping to the server
void *send_ping(void *arg) {
    // Get player ID
    int id = *(int *)arg;

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // Send the ping message
    char message[20];
    sprintf(message, "PING %d", id);
    if (send(sockfd, message, strlen(message), 0) == -1) {
        perror("send");
        exit(EXIT_FAILURE);
    }

    // Receive the ping response
    char response[20];
    if (recv(sockfd, response, sizeof(response), 0) == -1) {
        perror("recv");
        exit(EXIT_FAILURE);
    }

    // Parse the ping response
    int ping;
    sscanf(response, "PONG %d", &ping);

    // Update the player's ping
    pthread_mutex_lock(&players_mutex);
    players[id].ping = ping;
    pthread_mutex_unlock(&players_mutex);

    // Close the socket
    close(sockfd);

    return NULL;
}

// Function to print the player scores
void print_scores() {
    pthread_mutex_lock(&players_mutex);

    printf("Scores:\n");
    for (int i = 0; i < MAX_PLAYERS; i++) {
        printf("%s: %d\n", players[i].name, players[i].score);
    }

    pthread_mutex_unlock(&players_mutex);
}

// Main function
int main() {
    // Initialize the players
    for (int i = 0; i < MAX_PLAYERS; i++) {
        players[i].id = i;
        sprintf(players[i].name, "Player %d", i);
        players[i].score = 0;
        players[i].ping = 0;
    }

    // Create a thread for each player
    pthread_t threads[MAX_PLAYERS];
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (pthread_create(&threads[i], NULL, send_ping, &i) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Join the threads
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    // Print the player scores
    print_scores();

    return 0;
}