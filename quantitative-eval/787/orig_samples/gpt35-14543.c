//FormAI DATASET v1.0 Category: Network Ping Test ; Style: interoperable
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define ICMP_PACKET_SIZE 64
#define ICMP_HEADER_SIZE 8
#define MAX_HOPS 30

// ICMP packet structure
typedef struct icmp_packet
{
    uint8_t type; // ICMP packet type
    uint8_t code; // ICMP packet code
    uint16_t checksum; // ICMP packet checksum
    uint16_t id; // ICMP packet identifier
    uint16_t sequence_number; // ICMP packet sequence number
} icmp_packet_t;

// Calculates the checksum of the ICMP packet
uint16_t compute_checksum(void* data, size_t len)
{
    uint16_t* p = (uint16_t*)data;
    uint16_t checksum = 0;
    size_t i;

    for (i = 0; i < (len / 2); i++)
        checksum += p[i];

    if (len % 2 != 0)
        checksum += *((uint8_t*)data + len - 1); 

    while (checksum >> 16)
        checksum = (checksum & 0xFFFF) + (checksum >> 16);

    return (uint16_t)(~checksum);
}

// Sends an ICMP packet to a given host
void ping_host(char* hostname)
{
    struct hostent* host;
    struct sockaddr_in address;
    icmp_packet_t packet;
    ssize_t bytes_received;
    uint8_t buffer[ICMP_PACKET_SIZE];
    int timeout = 3000; // 3 second timeout

    // Get the IP address of the host
    host = gethostbyname(hostname);
    if (host == NULL)
    {
        fprintf(stderr, "Failed to resolve hostname: %s\n", hostname);
        return;
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(0);
    memcpy(&address.sin_addr, host->h_addr, host->h_length);

    // Create a raw socket for ICMP packets
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        fprintf(stderr, "Failed to create socket: %s\n", strerror(errno));
        return;
    }

    // Configure the ICMP packet
    memset(&packet, 0, sizeof(packet));
    packet.type = 8; // echo request
    packet.code = 0; // zero code for echo request
    packet.id = htons((uint16_t)getpid()); // unique identifier
    packet.sequence_number = htons(1); // start with sequence number 1
    packet.checksum = compute_checksum(&packet, sizeof(packet));

    // Send the ICMP packet to the host
    clock_t start_time = clock();
    int bytes_sent = sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr*)&address, sizeof(address));
    if (bytes_sent < 0)
    {
        fprintf(stderr, "Failed to send packet: %s\n", strerror(errno));
        close(sockfd);
        return;
    }

    // Wait for the response ICMP packet
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);
    struct timeval timeout_val;
    timeout_val.tv_sec = timeout / 1000;
    timeout_val.tv_usec = (timeout % 1000) * 1000;
    int select_retval = select(sockfd + 1, &readfds, NULL, NULL, &timeout_val);
    if (select_retval == -1)
    {
        fprintf(stderr, "Failed to select: %s\n", strerror(errno));
        close(sockfd);
        return;
    }
    else if (select_retval == 0)
    {
        printf("***HOST %s NOT RESPONDING***\n", hostname);
        close(sockfd);
        return;
    }

    // Receive the ICMP packet response
    bytes_received = recv(sockfd, buffer, ICMP_PACKET_SIZE, 0);
    if (bytes_received < 0)
    {
        fprintf(stderr, "Failed to receive packet: %s\n", strerror(errno));
        close(sockfd);
        return;
    }

    // Extract the type and code from the response packet
    icmp_packet_t* response_packet = (icmp_packet_t*)buffer;
    uint8_t response_type = response_packet->type;
    uint8_t response_code = response_packet->code;

    // Calculate the round-trip time and print it
    clock_t end_time = clock();
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000.0;
    printf("%-20s PING %d bytes: time=%.2f ms\n", hostname, bytes_received - ICMP_HEADER_SIZE, elapsed_time);

    close(sockfd);
}

// Traces a route to a given host
void traceroute(char* hostname)
{
    struct hostent* host;
    struct sockaddr_in address;
    icmp_packet_t packet;
    ssize_t bytes_received;
    uint8_t buffer[ICMP_PACKET_SIZE];
    int timeout = 3000; // 3 second timeout
    int ttl = 1;
    int should_exit = 0;

    // Get the IP address of the host
    host = gethostbyname(hostname);
    if (host == NULL)
    {
        fprintf(stderr, "Failed to resolve hostname: %s\n", hostname);
        return;
    }

    // Create a raw socket for ICMP packets
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        fprintf(stderr, "Failed to create socket: %s\n", strerror(errno));
        return;
    }

    // Set the TTL option for the socket
    if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
    {
        fprintf(stderr, "Failed to set TTL option: %s\n", strerror(errno));
        close(sockfd);
        return;
    }

    // Output the header for the traceroute
    printf("Tracing route to %s [%s]\n", hostname, inet_ntoa(*(struct in_addr*)host->h_addr));

    // Loop until we reach the destination or the maximum hop count
    while (!should_exit && ttl <= MAX_HOPS)
    {
        int i;
        int bytes_sent;

        // Configure the ICMP packet
        memset(&packet, 0, sizeof(packet));
        packet.type = 8; // echo request
        packet.code = 0; // zero code for echo request
        packet.id = htons((uint16_t)getpid()); // unique identifier
        packet.sequence_number = htons(1); // start with sequence number 1
        packet.checksum = compute_checksum(&packet, sizeof(packet));

        // Set the TTL for the socket
        if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
        {
            fprintf(stderr, "Failed to set TTL option: %s\n", strerror(errno));
            close(sockfd);
            return;
        }

        // Send the ICMP packet to the host
        clock_t start_time = clock();
        bytes_sent = sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr*)&address, sizeof(address));
        if (bytes_sent < 0)
        {
            fprintf(stderr, "Failed to send packet: %s\n", strerror(errno));
            close(sockfd);
            return;
        }

        // Wait for the response ICMP packet
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        struct timeval timeout_val;
        timeout_val.tv_sec = timeout / 1000;
        timeout_val.tv_usec = (timeout % 1000) * 1000;
        int select_retval = select(sockfd + 1, &readfds, NULL, NULL, &timeout_val);
        if (select_retval == -1)
        {
            fprintf(stderr, "Failed to select: %s\n", strerror(errno));
            close(sockfd);
            return;
        }
        else if (select_retval == 0)
        {
            printf("%2d  * * *\n", ttl);
            ttl++;
            continue;
        }

        // Receive the ICMP packet response
        bytes_received = recv(sockfd, buffer, ICMP_PACKET_SIZE, 0);
        if (bytes_received < 0)
        {
            fprintf(stderr, "Failed to receive packet: %s\n", strerror(errno));
            close(sockfd);
            return;
        }

        // Extract the type and code from the response packet
        icmp_packet_t* response_packet = (icmp_packet_t*)buffer;
        uint8_t response_type = response_packet->type;
        uint8_t response_code = response_packet->code;

        // Calculate the round-trip time and print it
        clock_t end_time = clock();
        double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000.0;

        // Print the hop information
        struct in_addr hop_address;
        memset(&hop_address, 0, sizeof(struct in_addr));
        memcpy(&hop_address.s_addr, buffer + ICMP_HEADER_SIZE + 12, sizeof(struct in_addr));
        printf("%2d  %s (%s)  %d ms\n", ttl, host->h_name, inet_ntoa(hop_address), (int)elapsed_time);

        // Check if we have reached the destination
        if (response_type == 0 && response_code == 0)
        {
            should_exit = 1;
        }

        ttl++;
    }

    close(sockfd);
}

// Main entry point of the program
int main(int argc, char* argv[])
{
    // Check the command-line arguments
    if (argc < 2)
    {
        printf("Usage: %s [options] destination\n\n", argv[0]);
        printf("Options:\n");
        printf("  -p    Run a ping test instead of a traceroute\n");
        return 1;
    }

    int use_ping = 0;

    // Parse the command-line arguments
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-p") == 0)
            use_ping = 1;
        else
            break;
    }

    char* destination = argv[argc - 1];

    // Run the appropriate test
    if (use_ping)
        ping_host(destination);
    else
        traceroute(destination);

    return 0;
}