//FormAI DATASET v1.0 Category: Firewall ; Style: dynamic
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080
#define MAX_BUFFER 1024

// Structure for rules
struct Rule {
    char sourceIP[20];
    char destinationIP[20];
    int sourcePort;
    int destinationPort;
    char protocol[10];
};

struct Client {
    int socket;
    struct sockaddr_in address;
};

// Array to hold all firewall rules
struct Rule rules[10];
int ruleCount = 0;

// Add new firewall rule
void addRule(char sourceIP[20], char destinationIP[20], int sourcePort, int destinationPort, char protocol[10]) {
    strcpy(rules[ruleCount].sourceIP, sourceIP);
    strcpy(rules[ruleCount].destinationIP, destinationIP);
    rules[ruleCount].sourcePort = sourcePort;
    rules[ruleCount].destinationPort = destinationPort;
    strcpy(rules[ruleCount].protocol, protocol);
    ruleCount++;
}

// Check if given rule matches the given packet data
int isMatch(char sourceIP[20], char destinationIP[20], int sourcePort, int destinationPort, char protocol[10]) {
    for(int i=0; i<ruleCount; i++) {
        if(strcmp(rules[i].sourceIP, sourceIP) == 0 && strcmp(rules[i].destinationIP, destinationIP) == 0 && 
        rules[i].sourcePort == sourcePort && rules[i].destinationPort == destinationPort && strcmp(rules[i].protocol, protocol) == 0) {
          return 1;
        }
    }
    return 0;
}

// Thread function to handle client
void *handleClient(void *args) {
    struct Client client = *((struct Client*)args); // Client details
    int socket = client.socket;

    // Receive packet data from client
    char buffer[MAX_BUFFER] = {0};
    read(socket, buffer, MAX_BUFFER);

    // Parse packet data
    char sourceIP[20], destinationIP[20], protocol[10];
    int sourcePort, destinationPort;
    sscanf(buffer, "%[^,],%d,%[^,],%d,%[^,]", sourceIP, &sourcePort, destinationIP, &destinationPort, protocol);

    // Check if packet data matches firewall rule
    if(!isMatch(sourceIP, destinationIP, sourcePort, destinationPort, protocol)) {
        // Send rejection message to client
        char *responseMessage = "Packet Rejected by Firewall!";
        send(socket, responseMessage, strlen(responseMessage), 0);
    } else {
        // Send success message to client
        char *responseMessage = "Packet Approved by Firewall!";
        send(socket, responseMessage, strlen(responseMessage), 0);
    }

    close(socket); // Close client socket
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    int serverSocket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Create server socket
    if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set server socket options
    if(setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Set server socket details
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind server socket to address and port
    if(bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if(listen(serverSocket, 3)) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Firewall listening on port %d...\n", PORT);

    // Start accepting incoming connections
    while(1) {
        int clientSocket;
        struct sockaddr_in clientAddress;
        pthread_t threadID;

        // Accept incoming connection
        if((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        // Create thread to handle client
        struct Client client;
        client.socket = clientSocket;
        client.address = clientAddress;
        pthread_create(&threadID, NULL, handleClient, (void*)&client);
    }

    return 0;
}

// Example usage:
// addRule("192.168.0.1", "192.168.0.2", 80, 8080, "TCP");
// addRule("192.168.0.2", "192.168.0.1", 8080, 80, "TCP");
// addRule("192.168.0.1", "192.168.0.3", 53, 53, "UDP");