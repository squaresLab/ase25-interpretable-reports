//Falcon-180B DATASET v1.0 Category: Chat server ; Style: Ken Thompson
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int g_server_socket;
int g_clients_socket[MAX_CLIENTS];
int g_clients_count;
pthread_t g_threads[MAX_CLIENTS];

void* handle_client(void* arg)
{
    int client_socket = *(int*)arg;
    char buffer[BUFFER_SIZE];

    while(1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);

        if(bytes_received <= 0)
        {
            break;
        }

        printf("Client %d sent: %s\n", client_socket, buffer);

        for(int i = 0; i < g_clients_count; i++)
        {
            if(g_clients_socket[i]!= client_socket)
            {
                send(g_clients_socket[i], buffer, strlen(buffer), 0);
            }
        }
    }

    close(client_socket);
    return NULL;
}

int main()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, MAX_CLIENTS);

    printf("Server started on port %d\n", PORT);

    while(1)
    {
        int client_socket = accept(server_socket, NULL, NULL);

        if(g_clients_count >= MAX_CLIENTS)
        {
            send(client_socket, "Server is full.\n", strlen("Server is full.\n"), 0);
            close(client_socket);
            continue;
        }

        g_clients_socket[g_clients_count] = client_socket;
        g_clients_count++;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void*)&client_socket);
    }

    return 0;
}