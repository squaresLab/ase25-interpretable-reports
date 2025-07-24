//Falcon-180B DATASET v1.0 Category: Wireless Network Scanner ; Style: asynchronous
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PACKET_SIZE 1024
#define MAX_INTERFACES 10
#define MAX_THREADS 10

int sock;
struct sockaddr_in server;
char *interfaces[MAX_INTERFACES];
int num_interfaces;
pthread_t threads[MAX_THREADS];

void *scanner(void *arg) {
    int interface_index = *(int *)arg;
    char buffer[PACKET_SIZE];
    while (1) {
        int bytes_received = recvfrom(sock, buffer, PACKET_SIZE, 0, NULL, NULL);
        if (bytes_received <= 0) {
            printf("Error receiving packet on interface %s\n", interfaces[interface_index]);
            break;
        }
        printf("Received packet on interface %s: %s\n", interfaces[interface_index], buffer);
    }
    return NULL;
}

void start_scanner(int interface_index) {
    pthread_create(&threads[interface_index], NULL, scanner, &interface_index);
}

void *listener(void *arg) {
    char buffer[PACKET_SIZE];
    while (1) {
        int bytes_received = recvfrom(sock, buffer, PACKET_SIZE, 0, NULL, NULL);
        if (bytes_received <= 0) {
            printf("Error receiving packet\n");
            break;
        }
        printf("Received packet: %s\n", buffer);
        for (int i = 0; i < num_interfaces; i++) {
            start_scanner(i);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc!= 3) {
        printf("Usage: %s <interface1> <interface2>... <interfaceN>\n", argv[0]);
        return 1;
    }
    num_interfaces = 0;
    for (int i = 1; i < argc; i++) {
        int result = if_nametoindex(argv[i]);
        if (result == 0) {
            printf("Error: Invalid interface name %s\n", argv[i]);
            return 1;
        }
        interfaces[num_interfaces++] = argv[i];
    }
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock == -1) {
        printf("Error creating socket: %s\n", strerror(errno));
        return 1;
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(12345);
    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == -1) {
        printf("Error binding socket: %s\n", strerror(errno));
        return 1;
    }
    printf("Listening for packets...\n");
    pthread_t listener_thread;
    pthread_create(&listener_thread, NULL, listener, NULL);
    pthread_join(listener_thread, NULL);
    return 0;
}