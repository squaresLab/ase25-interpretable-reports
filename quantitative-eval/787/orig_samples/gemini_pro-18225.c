//GEMINI-pro DATASET v1.0 Category: Network Ping Test ; Style: surprised
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define ICMP_ECHO_REQUEST 8
#define ICMP_ECHO_REPLY 0
#define ICMP_ECHO_HEADER_LEN 8

// Surprise! This program implements a basic ICMP ping utility in C.
// It allows you to send ICMP echo requests to a specified IP address
// and measure the time it takes to receive a response.

// Structure to represent an ICMP echo request header.
struct icmp_echo_header {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t identifier;
    uint16_t sequence_number;
    uint32_t timestamp;
};

// Function to calculate the checksum of an ICMP packet.
uint16_t calculate_checksum(void *buffer, size_t len) {
    uint16_t sum = 0;
    uint16_t *words = (uint16_t *)buffer;
    int words_count = len / sizeof(uint16_t);
    
    for (int i = 0; i < words_count; i++) {
        sum += words[i];
    }
    
    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    
    return ~sum;
}

// Function to create an ICMP echo request packet.
void create_icmp_echo_request(struct icmp_echo_header *header, uint16_t identifier, uint16_t sequence_number) {
    // Initialize the header fields.
    header->type = ICMP_ECHO_REQUEST;
    header->code = 0;
    header->checksum = 0;
    header->identifier = identifier;
    header->sequence_number = sequence_number;
    header->timestamp = time(NULL);
    
    // Calculate the checksum.
    header->checksum = calculate_checksum(header, ICMP_ECHO_HEADER_LEN);
}

// Function to send an ICMP echo request packet to a specified IP address.
int send_icmp_echo_request(struct sockaddr_in *addr, struct icmp_echo_header *header) {
    // Create a raw socket for sending ICMP packets.
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }
    
    // Set the socket options to allow sending ICMP packets.
    int on = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0) {
        perror("setsockopt");
        return -1;
    }
    
    // Send the ICMP echo request packet.
    if (sendto(sockfd, header, ICMP_ECHO_HEADER_LEN, 0, (struct sockaddr *)addr, sizeof(*addr)) < 0) {
        perror("sendto");
        return -1;
    }
    
    close(sockfd);
    return 0;
}

// Function to receive an ICMP echo reply packet.
int receive_icmp_echo_reply(struct sockaddr_in *addr, struct icmp_echo_header *header) {
    // Create a raw socket for receiving ICMP packets.
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }
    
    // Set a timeout on the socket so that it doesn't block forever.
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("setsockopt");
        return -1;
    }
    
    // Receive the ICMP echo reply packet.
    socklen_t addr_len = sizeof(*addr);
    if (recvfrom(sockfd, header, ICMP_ECHO_HEADER_LEN, 0, (struct sockaddr *)addr, &addr_len) < 0) {
        perror("recvfrom");
        return -1;
    }
    
    close(sockfd);
    return 0;
}

// Function to calculate the round-trip time for an ICMP echo request.
double calculate_round_trip_time(struct icmp_echo_header *request, struct icmp_echo_header *reply) {
    // Convert the timestamp to milliseconds.
    double request_timestamp = (double)request->timestamp * 1000.0;
    double reply_timestamp = (double)reply->timestamp * 1000.0;
    
    // Calculate the round-trip time.
    return reply_timestamp - request_timestamp;
}

// Function to print the results of a ping test.
void print_ping_results(struct sockaddr_in *addr, double round_trip_time) {
    // Convert the IP address to a string.
    char ip_address[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &addr->sin_addr, ip_address, sizeof(ip_address)) == NULL) {
        perror("inet_ntop");
        return;
    }
    
    // Print the results.
    printf("Ping %s (%s):\n", ip_address, inet_ntoa(addr->sin_addr));
    printf("  Round-trip time: %.3f ms\n", round_trip_time);
    printf("\n");
}

// Main function.
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <IP address>\n", argv[0]);
        return 1;
    }
    
    // Parse the IP address.
    struct sockaddr_in addr;
    if (inet_pton(AF_INET, argv[1], &addr.sin_addr) != 1) {
        printf("Invalid IP address: %s\n", argv[1]);
        return 1;
    }
    
    addr.sin_family = AF_INET;
    
    // Create an ICMP echo request header.
    struct icmp_echo_header request;
    create_icmp_echo_request(&request, 0, 0);
    
    // Send the ICMP echo request packet.
    if (send_icmp_echo_request(&addr, &request) < 0) {
        return 1;
    }
    
    // Receive the ICMP echo reply packet.
    struct icmp_echo_header reply;
    if (receive_icmp_echo_reply(&addr, &reply) < 0) {
        return 1;
    }
    
    // Calculate the round-trip time.
    double round_trip_time = calculate_round_trip_time(&request, &reply);
    
    // Print the results.
    print_ping_results(&addr, round_trip_time);
    
    return 0;
}