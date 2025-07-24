//GEMINI-pro DATASET v1.0 Category: Network Ping Test ; Style: relaxed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

// Define the maximum number of hops for the ICMP packet
#define MAX_HOPS 30

// Define the ICMP header structure
struct icmp_header {
    unsigned char type; // Message type (8 for echo request)
    unsigned char code; // Error code (0 for echo request)
    unsigned short checksum; // Checksum of the header and data
    unsigned short identifier; // Identifier of the request
    unsigned short sequence_number; // Sequence number of the request
    char data[52]; // Data portion of the packet
};

// Calculate the checksum of the ICMP packet
unsigned short calculate_checksum(struct icmp_header *header) {
    unsigned short *words = (unsigned short *)header;
    unsigned int sum = 0;
    int i;

    // Calculate the sum of all the 16-bit words in the header
    for (i = 0; i < sizeof(struct icmp_header) / sizeof(unsigned short); i++) {
        sum += words[i];
    }

    // Add the carry-over from the previous sum
    while (sum >> 16) {
        sum = (sum >> 16) + (sum & 0xFFFF);
    }

    // Return the one's complement of the sum
    return ~sum;
}

// Send an ICMP echo request packet
int send_ping(int sockfd, struct sockaddr_in *addr, int ttl) {
    struct icmp_header header;

    // Set the ICMP header fields
    header.type = 8; // Echo request
    header.code = 0; // Error code
    header.checksum = 0; // Calculate the checksum later
    header.identifier = getpid(); // Process ID
    header.sequence_number = 0; // Sequence number
    memset(header.data, 0, sizeof(header.data)); // Clear the data portion

    // Calculate the checksum of the header
    header.checksum = calculate_checksum(&header);

    // Set the TTL of the packet
    setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl));

    // Send the ICMP packet
    if (sendto(sockfd, &header, sizeof(header), 0, (struct sockaddr *)addr, sizeof(*addr)) < 0) {
        perror("sendto");
        return -1;
    }

    return 0;
}

// Receive an ICMP echo reply packet
int receive_ping(int sockfd, struct sockaddr_in *addr, int *ttl) {
    struct icmp_header header;
    struct sockaddr_in from_addr;
    socklen_t from_len = sizeof(from_addr);

    // Receive the ICMP packet
    if (recvfrom(sockfd, &header, sizeof(header), 0, (struct sockaddr *)&from_addr, &from_len) < 0) {
        perror("recvfrom");
        return -1;
    }

    // Get the TTL of the packet
    getsockopt(sockfd, IPPROTO_IP, IP_TTL, ttl, &from_len);

    // Check if the ICMP packet is an echo reply
    if (header.type != 0) {
        return -1;
    }

    // Check if the ICMP packet is from the correct source address
    if (strcmp(inet_ntoa(addr->sin_addr), inet_ntoa(from_addr.sin_addr)) != 0) {
        return -1;
    }

    return 0;
}

// Perform a ping test
int ping(char *hostname, int ttl) {
    struct sockaddr_in addr;
    int sockfd, i, ttl_received;
    struct timeval start, end;
    long elapsed_time;

    // Create a raw socket for sending and receiving ICMP packets
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        perror("socket");
        return -1;
    }

    // Resolve the hostname to an IP address
    if (inet_aton(hostname, &addr.sin_addr) == 0) {
        perror("inet_aton");
        return -1;
    }

    // Set the IP address and port of the destination address
    addr.sin_family = AF_INET;
    addr.sin_port = 0;

    // Send ICMP echo request packets
    for (i = 0; i < 3; i++) {
        // Send the ICMP echo request packet
        if (send_ping(sockfd, &addr, ttl) < 0) {
            close(sockfd);
            return -1;
        }

        // Get the start time
        gettimeofday(&start, NULL);

        // Receive the ICMP echo reply packet
        if (receive_ping(sockfd, &addr, &ttl_received) < 0) {
            close(sockfd);
            return -1;
        }

        // Get the end time
        gettimeofday(&end, NULL);

        // Calculate the elapsed time
        elapsed_time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;

        // Print the ping statistics
        printf("Ping: %s (%s) ttl=%d elapsed_time=%ld ms\n", hostname, inet_ntoa(addr.sin_addr), ttl_received, elapsed_time);
    }

    // Close the socket
    close(sockfd);

    return 0;
}

int main(int argc, char *argv[]) {
    int ttl;

    // Check if the hostname and TTL are specified
    if (argc != 3) {
        fprintf(stderr, "Usage: %s hostname ttl\n", argv[0]);
        return 1;
    }

    // Get the TTL
    ttl = atoi(argv[2]);

    // Perform the ping test
    if (ping(argv[1], ttl) < 0) {
        return 1;
    }

    return 0;
}