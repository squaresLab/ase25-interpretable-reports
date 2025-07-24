//GEMINI-pro DATASET v1.0 Category: Network Ping Test ; Style: synchronous
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#define MAX_HOPS 30
#define MAX_PACKET_SIZE 1024
#define ICMP_ECHO_REQUEST 8
#define ICMP_ECHO_REPLY 0
#define ICMP_MAX_ECHO_DATA_SIZE 56

// ICMP header structure
struct icmp_header {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t identifier;
    uint16_t sequence_number;
    uint8_t data[ICMP_MAX_ECHO_DATA_SIZE];
};

// ICMP packet structure
struct icmp_packet {
    struct icmp_header header;
    uint8_t data[MAX_PACKET_SIZE - sizeof(struct icmp_header)];
};

// Process a received ICMP packet
void process_icmp_packet(struct icmp_packet *packet, socklen_t addr_len, struct sockaddr *addr) {
    // Check if the ICMP packet is an ICMP echo reply
    if (packet->header.type != ICMP_ECHO_REPLY) {
        printf("Received non-ICMP echo reply packet.\n");
        return;
    }

    // Print the IP address and port of the sender
    char addr_str[INET6_ADDRSTRLEN];
    if (addr->sa_family == AF_INET) {
        struct sockaddr_in *addr_in = (struct sockaddr_in *)addr;
        inet_ntop(AF_INET, &addr_in->sin_addr, addr_str, sizeof(addr_str));
        printf("Received ICMP echo reply from %s:%d\n", addr_str, ntohs(addr_in->sin_port));
    } else if (addr->sa_family == AF_INET6) {
        struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)addr;
        inet_ntop(AF_INET6, &addr_in6->sin6_addr, addr_str, sizeof(addr_str));
        printf("Received ICMP echo reply from %s:%d\n", addr_str, ntohs(addr_in6->sin6_port));
    } else {
        printf("Received ICMP echo reply from unknown address type.\n");
    }

    // Print the sequence number and timestamp of the ICMP packet
    printf("Sequence number: %d\n", ntohs(packet->header.sequence_number));
    printf("Received time: %ld ms\n", (time(NULL) - packet->header.identifier) * 1000);
}

// Send an ICMP echo request packet
int send_icmp_packet(int sockfd, struct sockaddr *addr, socklen_t addr_len, int identifier, int sequence_number) {
    // Create an ICMP packet
    struct icmp_packet packet;
    memset(&packet, 0, sizeof(packet));

    // Fill in the ICMP header
    packet.header.type = ICMP_ECHO_REQUEST;
    packet.header.code = 0;
    packet.header.checksum = 0;
    packet.header.identifier = htons(identifier);
    packet.header.sequence_number = htons(sequence_number);

    // Calculate the ICMP checksum
    int checksum_len = sizeof(packet.header) + sizeof(packet.data);
    uint32_t checksum = 0;
    for (int i = 0; i < checksum_len; i += 2) {
        checksum += *(uint16_t *)((uint8_t *)&packet + i);
    }
    checksum = (checksum >> 16) + (checksum & 0xFFFF);
    checksum += (checksum >> 16);
    packet.header.checksum = ~htons(checksum);

    // Send the ICMP packet
    if (sendto(sockfd, &packet, sizeof(packet), 0, addr, addr_len) < 0) {
        perror("sendto");
        return -1;
    }

    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <hostname or IP address>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Create a raw socket for sending and receiving ICMP packets
    int sockfd = socket(AF_INET6, SOCK_RAW, IPPROTO_ICMPV6);
    if (sockfd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    // Set the socket options
    int on = 1;
    if (setsockopt(sockfd, IPPROTO_IPV6, IPV6_RECVPKTINFO, &on, sizeof(on)) < 0) {
        perror("setsockopt");
        return EXIT_FAILURE;
    }

    // Resolve the hostname or IP address
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMPV6;
    struct addrinfo *addr_info;
    if (getaddrinfo(argv[1], NULL, &hints, &addr_info) != 0) {
        perror("getaddrinfo");
        return EXIT_FAILURE;
    }

    // Get the destination address
    struct sockaddr *addr = addr_info->ai_addr;
    socklen_t addr_len = addr_info->ai_addrlen;

    // Set a timeout for receiving ICMP packets
    struct timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("setsockopt");
        return EXIT_FAILURE;
    }

    // Send an ICMP echo request packet
    int identifier = getpid();
    int sequence_number = 0;
    if (send_icmp_packet(sockfd, addr, addr_len, identifier, sequence_number) < 0) {
        return EXIT_FAILURE;
    }

    // Receive an ICMP echo reply packet
    struct icmp_packet packet;
    struct sockaddr addr_recv;
    socklen_t addr_recv_len = sizeof(addr_recv);
    if (recvfrom(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&addr_recv, &addr_recv_len) >= 0) {
        process_icmp_packet(&packet, addr_recv_len, &addr_recv);
    } else {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            printf("Timeout: no ICMP echo reply received.\n");
        } else {
            perror("recvfrom");
            return EXIT_FAILURE;
        }
    }

    // Close the socket
    close(sockfd);

    return EXIT_SUCCESS;
}