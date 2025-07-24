//FormAI DATASET v1.0 Category: Network Ping Test ; Style: secure
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define ICMP_ECHO_REQ 8 /* Type of ICMP requests */

#define PACKET_SIZE 64 /* Packet size for sending/receiving ICMP messages */

#define MAX_HOP 64 /* Maximum hops in traceroute */

#define MAX_TRIES 3 /* Maximum number of tries for each hop */

#define TIMEOUT_SEC 5 /* Time out for each try in seconds */

#define DEFAULT_PORT 0 /* Default port (0 = automatic) */

/* ICMP packet header structure */
struct icmp_header 
{
    unsigned char type; /* ICMP message type */
    unsigned char code; /* ICMP message sub-type */
    unsigned short checksum; /* ICMP header checksum */
    unsigned short id; /* ICMP packet ID (process ID) */
    unsigned short sequence; /* ICMP packet sequence number */
    struct timeval timestamp; /* ICMP packet timestamp */
};

/* IP address structure */
struct ip_address 
{
    unsigned char byte1;
    unsigned char byte2;
    unsigned char byte3;
    unsigned char byte4;
};

char *dns_lookup(char *, struct sockaddr_in *);
void send_ping(int, struct sockaddr_in *, struct icmp_header *, int);
struct timeval recv_ping(int, struct sockaddr_in *, struct ip_address *, int);
int icmp_checksum(struct icmp_header *, int);
int perform_traceroute(char *, int);
void print_error(char *);

/* Converts an IP address from dotted decimal notation to a struct ip_address */
int get_ip_address(char *ip_str, struct ip_address *ipaddr) 
{
    if (ipaddr == NULL) 
    {
        return -1;
    }

    if (inet_aton(ip_str, (struct in_addr *)ipaddr) == 0) 
    {
        return -1;
    }

    return 0;
}

/* Converts an IP address from a struct ip_address to dotted decimal notation */
void print_ip_address(struct ip_address ip) 
{
    printf("%d.%d.%d.%d", ip.byte1, ip.byte2, ip.byte3, ip.byte4);
}

/* Looks up a domain name and retrieves its IP address */
char *dns_lookup(char *host_ip, struct sockaddr_in *hostaddr) 
{
    struct hostent *host_entities;
    char *hostname = malloc(MAX_HOP), *ipaddr = malloc(MAX_HOP);

    if (get_ip_address(host_ip, (struct ip_address *)hostaddr) == 0) 
    {
        strcpy(ipaddr, host_ip);
        return ipaddr;
    }

    if ((host_entities = gethostbyaddr((char *)hostaddr, sizeof(struct sockaddr_in), AF_INET)) == NULL) 
    {
        return NULL;
    }

    strcpy(hostname, host_entities->h_name);
    strcpy(ipaddr, inet_ntoa(*(struct in_addr *)host_entities->h_addr));

    return ipaddr;
}

/* Sends an ICMP echo request */
void send_ping(int ping_sockfd, struct sockaddr_in *ping_addr, struct icmp_header *icmp_pkt, int sequence_no) 
{
    int pkt_size = 0;

    icmp_pkt->type = ICMP_ECHO_REQ;
    icmp_pkt->code = 0;
    icmp_pkt->id = getpid();

    /* Set packet timestamp */
    if (gettimeofday(&(icmp_pkt->timestamp), NULL) == -1) 
    {
        perror("gettimeofday");
        exit(EXIT_FAILURE);
    }

    icmp_pkt->sequence = sequence_no;
    icmp_pkt->checksum = icmp_checksum(icmp_pkt, sizeof(struct icmp_header));

    pkt_size = sendto(ping_sockfd, icmp_pkt, PACKET_SIZE, 0, (struct sockaddr *)ping_addr, sizeof(struct sockaddr_in));

    if (pkt_size == -1) 
    {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
}

/* Receives an ICMP echo reply */
struct timeval recv_ping(int ping_sockfd, struct sockaddr_in *ping_addr, struct ip_address *ipaddr, int sequence_no) 
{
    struct timeval recv_time, timeout;

    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = 0;

    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(ping_sockfd, &read_fds);

    int select_result = select(ping_sockfd + 1, &read_fds, NULL, NULL, &timeout);
    if (select_result == -1) 
    {
        perror("select");
        exit(EXIT_FAILURE);
    } 
    else if (select_result == 0) 
    {
        printf("*\t");
        fflush(stdout);
        return timeout;
    }

    struct sockaddr_in res_addr;
    socklen_t len = sizeof(res_addr);
    unsigned char recv_buf[PACKET_SIZE];
    int recv_size, iphdr_len;

    if ((recv_size = recvfrom(ping_sockfd, recv_buf, PACKET_SIZE, 0, (struct sockaddr *)&res_addr, &len)) == -1) 
    {
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }

    /* Get IP header length */
    iphdr_len = (int)(((unsigned char)recv_buf[0]) & 0x0F) << 2;

    /* Fill in the IP address of the hop */
    ipaddr->byte1 = (unsigned char)recv_buf[iphdr_len + 12];
    ipaddr->byte2 = (unsigned char)recv_buf[iphdr_len + 13];
    ipaddr->byte3 = (unsigned char)recv_buf[iphdr_len + 14];
    ipaddr->byte4 = (unsigned char)recv_buf[iphdr_len + 15];

    /* Fill in the timestamp of the received packet */
    if (gettimeofday(&recv_time, NULL) == -1) 
    {
        perror("gettimeofday");
        exit(EXIT_FAILURE);
    }

    /* Print the time taken by the received packet to reach the host */
    long int rtt = (recv_time.tv_sec - ((struct icmp_header *)recv_buf)->timestamp.tv_sec) * 1000000L + (recv_time.tv_usec - ((struct icmp_header *)recv_buf)->timestamp.tv_usec);
    printf("%ld.%03lds\t", rtt / 1000, rtt % 1000);
    fflush(stdout);

    return recv_time;
}

/* Calculates the checksum of an ICMP packet */
int icmp_checksum(struct icmp_header *icmp_pkt, int pkt_size) 
{
    unsigned short *buf = (unsigned short *)icmp_pkt;
    unsigned int sum = 0;

    /* Calculate checksum using 16-bit words */
    for (sum = 0; pkt_size > 1; pkt_size -= 2) 
    {
        sum += *buf++;

        if (sum & 0x80000000)
        {
            sum = (sum & 0xFFFF) + (sum >> 16);
        }
    }

    if (pkt_size == 1)
    {
        sum += *(unsigned char *)buf;
    }

    while (sum >> 16)
    {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    /* Take one's complement of the sum */
    return ~sum;
}

/* Performs a traceroute to a given IP address or domain */
int perform_traceroute(char *ip_addr, int port) 
{
    int i, j, ping_sockfd;
    struct sockaddr_in ping_addr;
    struct ip_address hop_ip[MAX_HOP];
    struct icmp_header icmp_req;
    struct timeval recv_time;

    if ((ping_sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1) 
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (port == DEFAULT_PORT) 
    {
        port = (getuid() != 0) ? 1023 : 0;
    }

    memset(&ping_addr, 0, sizeof(ping_addr));
    ping_addr.sin_family = AF_INET;
    ping_addr.sin_port = htons(port);

    char *host_name = dns_lookup(ip_addr, &ping_addr);
    printf("Traceroute to %s (%s), %d hops max\n", host_name, ip_addr, MAX_HOP);
    fflush(stdout);

    for (i = 1; i <= MAX_HOP; i++) 
    {
        printf("%d.\t", i);

        /* Set hop IP address */
        hop_ip[i-1].byte1 = hop_ip[i-1].byte2 = hop_ip[i-1].byte3 = hop_ip[i-1].byte4 = 0;
        ping_addr.sin_addr.s_addr = *((unsigned int *)(&hop_ip[i - 1]));

        for (j = 0; j < MAX_TRIES; j++) 
        {
            send_ping(ping_sockfd, &ping_addr, &icmp_req, i);

            /* Receive ICMP echo replies */
            recv_time = recv_ping(ping_sockfd, &ping_addr, &hop_ip[i-1], i);

            /* Check if the ICMP packet is a reply to a transmitted packet */
            if (icmp_req.id == getpid() && icmp_req.sequence == i) 
            {
                break;
            }
        }

        printf("\n");

        /* Exit if it's the final destination */
        if (hop_ip[i-1].byte1 == ((struct sockaddr_in *)&ping_addr)->sin_addr.s_addr >> 24
            && hop_ip[i-1].byte2 == (((struct sockaddr_in *)&ping_addr)->sin_addr.s_addr & 0x00FFFFFF) >> 16) 
        {
            break;
        }
    }

    close(ping_sockfd);
    return 0;
}

/* Prints errors */
void print_error(char *progname) 
{
    fprintf(stderr, "Usage: %s [-p port] hostname/IPaddress\n", progname);
}

/* Main function */
int main(int argc, char **argv) 
{
    int port = DEFAULT_PORT;
    char *progname = argv[0];

    /* Parse command line arguments */
    int i;
    for (i = 1; i < argc; ++i) 
    {
        if (strcmp(argv[i], "-p") == 0) 
        {
            if (++i < argc) 
            {
                port = atoi(argv[i]);
            } 
            else 
            {
                print_error(progname);
                return -1;
            }
        } 
        else 
        {
            break;
        }
    }

    if (i < argc - 1) 
    {
        print_error(progname);
        return -1;
    }

    char *ip_addr = argv[argc-1];

    return perform_traceroute(ip_addr, port);
}