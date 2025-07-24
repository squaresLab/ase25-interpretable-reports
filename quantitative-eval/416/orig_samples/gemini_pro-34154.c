//GEMINI-pro DATASET v1.0 Category: Electronic Voting System ; Style: multiplayer
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CANDIDATES 10
#define MAX_VOTERS 100
#define MAX_VOTES 10
#define PORT 8080

typedef struct candidate {
    char name[100];
    int votes;
} candidate_t;

typedef struct voter {
    char name[100];
    int votes[MAX_VOTES];
} voter_t;

candidate_t candidates[MAX_CANDIDATES];
voter_t voters[MAX_VOTERS];
int num_candidates = 0;
int num_voters = 0;

int server_socket;

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;

    voter_t voter;
    recv(client_socket, &voter, sizeof(voter), 0);

    // Check if voter has already voted
    int voted = 0;
    for (int i = 0; i < num_voters; i++) {
        if (strcmp(voter.name, voters[i].name) == 0) {
            voted = 1;
            break;
        }
    }

    // If voter has not already voted, add their votes to the candidates
    if (!voted) {
        for (int i = 0; i < MAX_VOTES; i++) {
            if (voter.votes[i] >= 0 && voter.votes[i] < num_candidates) {
                candidates[voter.votes[i]].votes++;
            }
        }

        // Add voter to list of voters
        voters[num_voters] = voter;
        num_voters++;
    }

    // Send the updated candidate list to the client
    send(client_socket, &candidates, sizeof(candidates), 0);

    close(client_socket);

    return NULL;
}

int main() {
    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(1);
    }

    // Set server socket to be reusable
    int reuseaddr = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

    // Bind server socket to port
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("bind");
        exit(1);
    }

    // Listen on server socket
    if (listen(server_socket, 5) == -1) {
        perror("listen");
        exit(1);
    }

    // Accept connections from clients
    while (1) {
        int client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            perror("accept");
            exit(1);
        }

        // Create a new thread to handle the client
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, &client_socket) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    // Close server socket
    close(server_socket);

    return 0;
}