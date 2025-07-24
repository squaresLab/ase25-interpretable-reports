//FormAI DATASET v1.0 Category: Wi-Fi Signal Strength Analyzer ; Style: protected
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <net/if.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 1337
#define PACKET_SIZE 1024

pthread_mutex_t counter_lock;
int counter;

void *packet_handler(void *arg);

int main(int argc, char const *argv[]) {

    if (argc < 2) {
        printf("Usage: ./analyzer [INTERFACE]\n");
        printf("Example: ./analyzer eth0\n");
        return -1;
    }

    int sock_fd;
    struct sockaddr_in serv_addr;
    pthread_t thread_id;

    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("socket");
        return -1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        return -1;
    }

    if (bind(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
        return -1;
    }

    printf("[+] Analyzer started on interface: %s\n", argv[1]);

    while (1) {
        char buffer[PACKET_SIZE];
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        int packet_size = recvfrom(sock_fd, buffer, PACKET_SIZE, 0, (struct sockaddr *) &client_addr, &client_addr_len);
        if (packet_size < 0) {
            perror("recvfrom");
            continue;
        }

        // Create a new thread to process the packet
        pthread_create(&thread_id, NULL, packet_handler, buffer);
    }
    return 0;
}

void *packet_handler(void *arg) {
    char *buffer = (char *) arg; 

    // Determine signal strength from packet
    int signal_strength = (int8_t) buffer[22];

    // Lock the counter variable to avoid race condition
    pthread_mutex_lock(&counter_lock);
    counter++;

    // Print the counter and signal strength
    printf("[+] Packet %d: Signal strength: %d dBm\n", counter, signal_strength);

    // Unlock the counter variable
    pthread_mutex_unlock(&counter_lock);
    return NULL;
}