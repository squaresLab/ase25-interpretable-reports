//GEMINI-pro DATASET v1.0 Category: Chat server ; Style: configurable
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 10
#define MAX_MSG_SIZE 1024

// Server configuration options
typedef struct {
    int port;
    int backlog;
    int timeout;
} ServerConfig;

// Client information
typedef struct {
    int socket;
    struct sockaddr_in addr;
    char name[32];
} Client;

// Server state
typedef struct {
    int num_clients;
    Client clients[MAX_CLIENTS];
    fd_set rfds;
} ServerState;

// Function prototypes
ServerConfig load_config(const char *filename);
void start_server(ServerConfig config);
void handle_client(ServerState *state, int client_index);

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <config_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    ServerConfig config = load_config(argv[1]);
    start_server(config);

    return EXIT_SUCCESS;
}

ServerConfig load_config(const char *filename) {
    ServerConfig config;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fscanf(fp, "port %d\nbacklog %d\ntimeout %d", &config.port, &config.backlog, &config.timeout);
    fclose(fp);

    return config;
}

void start_server(ServerConfig config) {
    int server_socket;
    struct sockaddr_in server_addr;

    // Create a server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set server address and port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(config.port);

    // Bind the server socket to the address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, config.backlog) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Set up the server state
    ServerState state;
    state.num_clients = 0;
    FD_ZERO(&state.rfds);
    FD_SET(server_socket, &state.rfds);

    // Main server loop
    while (1) {
        // Wait for activity on any of the sockets
        fd_set rfds_copy = state.rfds;
        struct timeval tv;
        tv.tv_sec = config.timeout;
        tv.tv_usec = 0;
        int activity = select(FD_SETSIZE, &rfds_copy, NULL, NULL, &tv);

        if (activity == -1) {
            perror("select");
            exit(EXIT_FAILURE);
        } else if (activity == 0) {
            // Timeout
            continue;
        }

        // Accept new connections
        if (FD_ISSET(server_socket, &rfds_copy)) {
            struct sockaddr_in client_addr;
            socklen_t client_addr_len = sizeof(client_addr);
            int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
            if (client_socket == -1) {
                perror("accept");
                continue;
            }

            // Add the new client to the server state
            if (state.num_clients < MAX_CLIENTS) {
                Client client;
                client.socket = client_socket;
                client.addr = client_addr;
                strcpy(client.name, "Anonymous");
                state.clients[state.num_clients++] = client;
                FD_SET(client_socket, &state.rfds);
            } else {
                // Too many clients connected
                send(client_socket, "Server is full\n", 15, 0);
                close(client_socket);
            }
        }

        // Handle activity on existing clients
        for (int i = 0; i < state.num_clients; i++) {
            if (FD_ISSET(state.clients[i].socket, &rfds_copy)) {
                handle_client(&state, i);
            }
        }
    }
}

void handle_client(ServerState *state, int client_index) {
    Client *client = &state->clients[client_index];

    char buffer[MAX_MSG_SIZE];
    int bytes_received = recv(client->socket, buffer, MAX_MSG_SIZE - 1, 0);
    if (bytes_received == -1) {
        perror("recv");
        close(client->socket);
        FD_CLR(client->socket, &state->rfds);
        state->num_clients--;
        return;
    } else if (bytes_received == 0) {
        // Client closed the connection
        close(client->socket);
        FD_CLR(client->socket, &state->rfds);
        state->num_clients--;
        return;
    }

    buffer[bytes_received] = '\0';

    // Process the client's message
    if (strncmp(buffer, "/name ", 6) == 0) {
        // Client is setting their name
        strcpy(client->name, buffer + 6);
        send(client->socket, "Name set\n", 9, 0);
    } else if (strncmp(buffer, "/quit", 5) == 0) {
        // Client is quitting
        send(client->socket, "Goodbye\n", 8, 0);
        close(client->socket);
        FD_CLR(client->socket, &state->rfds);
        state->num_clients--;
    } else {
        // Client is sending a message
        char message[MAX_MSG_SIZE];
        snprintf(message, MAX_MSG_SIZE, "%s: %s", client->name, buffer);
        for (int i = 0; i < state->num_clients; i++) {
            if (i != client_index) {
                send(state->clients[i].socket, message, strlen(message), 0);
            }
        }
    }
}