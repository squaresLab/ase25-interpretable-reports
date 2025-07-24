//FormAI DATASET v1.0 Category: Network Topology Mapper ; Style: Ken Thompson
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_PACKET_SIZE 65536

struct ip_hdr {
    uint8_t version_ihl;
    uint8_t tos;
    uint16_t len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t src_addr;
    uint32_t dst_addr;
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <IP address>\n", argv[0]);
        return 1;
    }

    // Create a raw socket to interact with IP headers
    int sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock_fd < 0) {
        perror("socket()");
        return 1;
    }

    // Set the IP_HDRINCL option to include the IP header
    int one = 1;
    if (setsockopt(sock_fd, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
        perror("setsockopt()");
        return 1;
    }

    // Create an IP header
    struct ip_hdr ip_header;
    memset(&ip_header, 0, sizeof(ip_header));
    ip_header.version_ihl = 0x45;
    ip_header.tos = 0;
    ip_header.len = htons(sizeof(ip_header));
    ip_header.id = htons(54321);
    ip_header.frag_off = htons(0);
    ip_header.ttl = 255;
    ip_header.protocol = IPPROTO_ICMP;
    ip_header.src_addr = inet_addr("192.168.0.1");
    ip_header.dst_addr = inet_addr(argv[1]);
    ip_header.checksum = 0;

    // Compute the IP header checksum
    uint32_t sum = 0;
    uint16_t* s = (uint16_t*) &ip_header;
    for (int i = 0; i < sizeof(ip_header) / 2; i++) {
        sum += s[i];
    }
    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }
    ip_header.checksum = ~htons(sum);

    // Send the packet
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = 0;
    sin.sin_addr.s_addr = ip_header.dst_addr;

    if (sendto(sock_fd, &ip_header, sizeof(ip_header), 0, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        perror("sendto()");
        return 1;
    }

    // Receive the packet
    uint8_t buffer[MAX_PACKET_SIZE];
    ssize_t n = recv(sock_fd, buffer, sizeof(buffer), 0);
    if (n < 0) {
        perror("recv()");
        return 1;
    }

    // Analyze the packet
    struct ip_hdr* received_ip_header = (struct ip_hdr*) buffer;
    printf("ICMP reply from %s\n", inet_ntoa(*(struct in_addr*)&received_ip_header->src_addr));

    return 0;
}