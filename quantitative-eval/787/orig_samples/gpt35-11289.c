//FormAI DATASET v1.0 Category: Network Ping Test ; Style: automated
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>

#define PING_PACKET_SIZE 64
#define PORT_NUMBER 0
#define MAX_PACK_SIZE 1024
#define MAX_TIMEOUT 1

struct icmp_echo_packet {
    unsigned char type;
    unsigned char code;
    unsigned short checksum;
    unsigned short identifier;
    unsigned short seq_no;
};

uint16_t calculate_checksum(uint8_t *data, size_t len)
{
    uint16_t *p = (uint16_t *)data;
    uint16_t answer = 0;
    size_t i;

    for (i = 0; i < len - 1; i += 2) {
        answer += htons(p[i/2]);
    }

    if (i == len - 1) {
        uint16_t tmp = htons(data[len - 1]);
        answer += tmp;
    }

    answer = (answer >> 16) + (answer & 0xffff);
    answer += (answer >> 16);

    return ~answer;
}

void send_ping_packet(int sockfd, const struct sockaddr *dest_addr, uint16_t seq_no)
{
    struct icmp_echo_packet icmp_packet;
    size_t icmp_packet_size = sizeof(icmp_packet);

    memset(&icmp_packet, 0x00, icmp_packet_size);

    icmp_packet.type = 8;
    icmp_packet.code = 0;
    icmp_packet.checksum = calculate_checksum((uint8_t *)&icmp_packet, icmp_packet_size);
    icmp_packet.seq_no = htons(seq_no);
    icmp_packet.identifier = htons(getpid());

    if (sendto(sockfd, &icmp_packet, icmp_packet_size, 0, dest_addr, sizeof(*dest_addr)) < 0) {
        perror("sendto");
    } else {
        printf("Request Packet sent...\n");
    }
}

double calculate_time_diff(struct timeval start, struct timeval end)
{
    return ((double)(end.tv_sec - start.tv_sec) * 1000.0) + ((double)(end.tv_usec - start.tv_usec) / 1000.0);
}

void receive_ping_packet(int sockfd, struct sockaddr *src_addr, uint16_t seq_no)
{
    struct icmp_echo_packet recv_buff[MAX_PACK_SIZE];
    size_t recv_buff_size = sizeof(recv_buff);
    struct timeval time_sent, time_recv;
    double rtt;
    socklen_t src_addr_len;
    fd_set readfds;

    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);

    struct timeval timeout;
    timeout.tv_sec = MAX_TIMEOUT;
    timeout.tv_usec = 0;

    int ready = select(sockfd + 1, &readfds, NULL, NULL, &timeout);
    switch (ready) {
        case -1:
            perror("select error");
            break;

        case 0:
            printf("Timeout error: no packet received.\n");
            break;

        default:
            if (FD_ISSET(sockfd, &readfds)) {
                if (recvfrom(sockfd, &recv_buff, recv_buff_size, 0, src_addr, &src_addr_len) < 0) {
                    perror("recvfrom");
                } else {
                    gettimeofday(&time_recv, NULL);
                    struct icmp_echo_packet *recv_packet = (struct icmp_echo_packet *)recv_buff;
                    if (ntohs(recv_packet->seq_no) == seq_no && ntohs(recv_packet->identifier) == getpid()) {
                        rtt = calculate_time_diff(time_sent, time_recv);
                        printf("Reply Packet received in %.3f ms\n", rtt);
                    } else {
                        printf("Received packet is not for this task.\n");
                    }
                }
            }
            break;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: <Program Name> <IP Address>\n");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in dest_addr;
    struct hostent *host_ptr;
    int sockfd;
    int ttl_val = 64;
    uint16_t seq_no = 0;

    if ((host_ptr = gethostbyname(argv[1])) == NULL) {
        perror("gethostbyname");
        exit(EXIT_FAILURE);
    }

    memset(&dest_addr, 0x00, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT_NUMBER);
    dest_addr.sin_addr = *((struct in_addr *)host_ptr->h_addr);

    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl_val, sizeof(ttl_val))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    while (1) {
        seq_no++;
        send_ping_packet(sockfd, (struct sockaddr *)&dest_addr, seq_no);
        receive_ping_packet(sockfd, (struct sockaddr *)&dest_addr, seq_no);
        sleep(2);
    }

    return 0;
}