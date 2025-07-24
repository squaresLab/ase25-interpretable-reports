//FormAI DATASET v1.0 Category: Chat server ; Style: Ada Lovelace
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 8080

int clients[20]; // an array to keep track of all the connected clients
int num_clients = 0; // keep track of the number of clients currently connected

void *handle_client(void *client_socket)
{
    int sock = *(int *)client_socket;

    char buffer[1024] = {0};
    char incoming_message[1024] = {0};

    // add the new client socket to the list of clients
    clients[num_clients++] = sock;

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        memset(incoming_message, 0, sizeof(incoming_message));

        // read the incoming message from the current client
        int valread = read(sock, incoming_message, 1024);

        // if the client has disconnected, stop handling incoming messages
        if (valread == 0)
        {
            for (int i = 0; i < num_clients; i++)
            {
                if (clients[i] == sock)
                {
                    for (int j = i; j < num_clients - 1; j++)
                    {
                        clients[j] = clients[j + 1];
                    }

                    num_clients--;
                    break;
                }
            }

            break;
        }

        // send the incoming message to all connected clients (except the sender)
        for (int i = 0; i < num_clients; i++)
        {
            if (clients[i] != sock)
            {
                send(clients[i], incoming_message, strlen(incoming_message), 0);
            }
        }
    }

    // close the client socket once the while loop has finished
    close(sock);

    return NULL;
}

int main(int argc, char const *argv[])
{
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    // create a TCP socket for the server
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // set socket options to allow reuse of the address and port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // bind the socket to the specified address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // listen for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        int new_socket;
        struct sockaddr_in client_address;
        int addrlen = sizeof(client_address);

        // accept incoming connection requests
        if ((new_socket = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        pthread_t thread_id;

        // create a new thread to handle each connected client
        if (pthread_create(&thread_id, NULL, handle_client, (void *)&new_socket) < 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}