//GEMINI-pro DATASET v1.0 Category: Network Ping Test ; Style: futuristic
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

// Define the maximum number of bytes to send and receive
#define MAX_BYTES 1024

// Define the types of messages that can be sent
#define ICMP_ECHO_REQUEST 8
#define ICMP_ECHO_REPLY 0

// Define the structure of an ICMP packet
struct icmp_packet {
  uint8_t type;
  uint8_t code;
  uint16_t checksum;
  uint16_t identifier;
  uint16_t sequence_number;
  uint8_t data[MAX_BYTES];
};

// Define the structure of an IP packet
struct ip_packet {
  uint8_t version_and_header_length;
  uint8_t type_of_service;
  uint16_t total_length;
  uint16_t identification;
  uint16_t flags_and_fragment_offset;
  uint8_t time_to_live;
  uint8_t protocol;
  uint16_t header_checksum;
  uint32_t source_ip_address;
  uint32_t destination_ip_address;
  uint8_t data[MAX_BYTES];
};

// Define the structure of an Ethernet frame
struct ethernet_frame {
  uint8_t destination_mac_address[6];
  uint8_t source_mac_address[6];
  uint16_t type;
  uint8_t data[MAX_BYTES];
};

// Function to calculate the checksum of an ICMP packet
uint16_t calculate_checksum(struct icmp_packet *packet) {
  uint32_t sum = 0;
  uint16_t *words = (uint16_t *)packet;
  int i;

  for (i = 0; i < sizeof(struct icmp_packet) / sizeof(uint16_t); i++) {
    sum += words[i];
  }

  while (sum >> 16) {
    sum = (sum >> 16) + (sum & 0xffff);
  }

  return ~sum;
}

// Function to send an ICMP echo request packet
int send_icmp_echo_request(int sockfd, struct sockaddr_in *addr, struct icmp_packet *packet) {
  // Set the ICMP packet type to echo request
  packet->type = ICMP_ECHO_REQUEST;

  // Set the ICMP packet code to 0
  packet->code = 0;

  // Set the ICMP packet checksum to 0
  packet->checksum = 0;

  // Calculate the ICMP packet checksum
  packet->checksum = calculate_checksum(packet);

  // Send the ICMP echo request packet
  if (sendto(sockfd, packet, sizeof(struct icmp_packet), 0, (struct sockaddr *)addr, sizeof(struct sockaddr_in)) < 0) {
    perror("sendto");
    return -1;
  }

  return 0;
}

// Function to receive an ICMP echo reply packet
int receive_icmp_echo_reply(int sockfd, struct sockaddr_in *addr, struct icmp_packet *packet) {
  // Receive the ICMP echo reply packet
  if (recvfrom(sockfd, packet, sizeof(struct icmp_packet), 0, (struct sockaddr *)addr, sizeof(struct sockaddr_in)) < 0) {
    perror("recvfrom");
    return -1;
  }

  return 0;
}

// Function to print the results of a ping test
void print_ping_results(struct icmp_packet *packet, struct sockaddr_in *addr, double time) {
  // Get the IP address of the host that was pinged
  char *host_ip_address = inet_ntoa(addr->sin_addr);

  // Print the ping results
  printf("Ping %s (%s): bytes=%d time=%.3fms\n", host_ip_address, host_ip_address, sizeof(struct icmp_packet), time);
}

// Main function
int main(int argc, char **argv) {
  // Check if the user has specified a hostname or IP address
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <hostname or IP address>\n", argv[0]);
    exit(1);
  }

  // Create a raw socket for sending and receiving ICMP packets
  int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (sockfd < 0) {
    perror("socket");
    exit(1);
  }

  // Set the socket options to allow sending and receiving raw ICMP packets
  int on = 1;
  if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0) {
    perror("setsockopt");
    exit(1);
  }

  // Get the IP address of the host that was pinged
  struct hostent *host = gethostbyname(argv[1]);
  if (host == NULL) {
    perror("gethostbyname");
    exit(1);
  }

  // Create an ICMP echo request packet
  struct icmp_packet packet;

  // Send an ICMP echo request packet
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr = *(struct in_addr *)host->h_addr;
  addr.sin_port = 0;

  if (send_icmp_echo_request(sockfd, &addr, &packet) < 0) {
    perror("send_icmp_echo_request");
    exit(1);
  }

  // Receive an ICMP echo reply packet
  struct timeval start_time, end_time;

  gettimeofday(&start_time, NULL);

  if (receive_icmp_echo_reply(sockfd, &addr, &packet) < 0) {
    perror("receive_icmp_echo_reply");
    exit(1);
  }

  gettimeofday(&end_time, NULL);

  // Calculate the time it took for the ICMP echo request and reply packets to be exchanged
  double time = (end_time.tv_sec - start_time.tv_sec) * 1000.0 + (end_time.tv_usec - start_time.tv_usec) / 1000.0;

  // Print the ping results
  print_ping_results(&packet, &addr, time);

  // Close the socket
  close(sockfd);

  return 0;
}