//FormAI DATASET v1.0 Category: Chat server ; Style: visionary
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CHATROOMS 10
#define MAX_USERS 100
#define BUFFER_SIZE 1024

typedef struct {
    int id;
    char name[20];
    int active_chatroom;
    int socket_id;
    int is_admin;
} User;

typedef struct {
    int id;
    char name[20];
    int user_count;
    User users[MAX_USERS];
} Chatroom;

Chatroom chatrooms[MAX_CHATROOMS];
int chatroom_count = 0;
int user_count = 0;

void add_user(User user) {
    chatrooms[user.active_chatroom].users[chatrooms[user.active_chatroom].user_count] = user;
    chatrooms[user.active_chatroom].user_count++;
    user_count++;
}

void remove_user(User user) {
    for (int i = 0; i < chatrooms[user.active_chatroom].user_count; i++) {
        if (chatrooms[user.active_chatroom].users[i].id == user.id) {
            chatrooms[user.active_chatroom].users[i] = chatrooms[user.active_chatroom].users[chatrooms[user.active_chatroom].user_count - 1];
            chatrooms[user.active_chatroom].user_count--;
            user_count--;
            return;
        }
    }
}

void send_to_chatroom(int chatroom_id, char* message, int sender_id) {
    for (int i = 0; i < chatrooms[chatroom_id].user_count; i++) {
        if (chatrooms[chatroom_id].users[i].id != sender_id) {
            send(chatrooms[chatroom_id].users[i].socket_id, message, strlen(message), 0);
        }
    }
}

void send_to_user(User user, char* message) {
    send(user.socket_id, message, strlen(message), 0);
}

void list_chatrooms(int socket_id) {
    char buffer[BUFFER_SIZE] = "Available chatrooms:\n";
    for (int i = 0; i < chatroom_count; i++) {
        char chatroom_info[50];
        sprintf(chatroom_info, "%d. %s (%d users)\n", chatrooms[i].id, chatrooms[i].name, chatrooms[i].user_count);
        strcat(buffer, chatroom_info);
    }
    send(socket_id, buffer, strlen(buffer), 0);
}

void list_users(int chatroom_id, int socket_id) {
    char buffer[BUFFER_SIZE] = "Users in chatroom:\n";
    for (int i = 0; i < chatrooms[chatroom_id].user_count; i++) {
        char user_info[50];
        sprintf(user_info, "%s%s\n", chatrooms[chatroom_id].users[i].name, chatrooms[chatroom_id].users[i].is_admin ? " (admin)" : "");
        strcat(buffer, user_info);
    }
    send(socket_id, buffer, strlen(buffer), 0);
}

void* handle_client(void* client_socket_id) {
    int socket_id = *(int*)client_socket_id;
    User user;
    user.id = user_count;
    strcpy(user.name, "Unknown");
    user.active_chatroom = -1;
    user.socket_id = socket_id;
    user.is_admin = 0;

    add_user(user);
    char buffer[BUFFER_SIZE];

    char welcome_message[BUFFER_SIZE] = "Welcome to the chat server!\n";
    send(socket_id, welcome_message, strlen(welcome_message), 0);

    while (1) {
        recv(socket_id, buffer, BUFFER_SIZE, 0); // receive message from client
        char* message = strtok(buffer, "\n");

        if (strcmp(message, "/list") == 0) {
            list_chatrooms(socket_id);
        }
        else if (strncmp(message, "/join ", 6) == 0) {
            int chatroom_id = atoi(message + 6);
            if (chatroom_id < 0 || chatroom_id >= chatroom_count) {
                send_to_user(user, "Invalid chatroom ID.");
            }
            else {
                char join_message[BUFFER_SIZE];
                sprintf(join_message, "%s has joined the chat.\n", user.name);
                send_to_chatroom(user.active_chatroom, join_message, user.id);
                remove_user(user);
                user.active_chatroom = chatroom_id;
                add_user(user);
                char joined_message[BUFFER_SIZE];
                sprintf(joined_message, "You have joined chatroom %s.\n", chatrooms[chatroom_id].name);
                send_to_user(user, joined_message);
            }
        }
        else if (strncmp(message, "/name ", 6) == 0) {
            strcpy(user.name, message + 6);
            char name_set_message[BUFFER_SIZE];
            sprintf(name_set_message, "Your name is now %s.\n", user.name);
            send_to_user(user, name_set_message);
        }
        else if (strncmp(message, "/kick ", 6) == 0 && user.is_admin) {
            int kicked_user_id = atoi(message + 6);
            if (kicked_user_id < 0 || kicked_user_id >= chatrooms[user.active_chatroom].user_count) {
                send_to_user(user, "Invalid user ID.");
            }
            else {
                User kicked_user = chatrooms[user.active_chatroom].users[kicked_user_id];
                char kick_message[BUFFER_SIZE];
                sprintf(kick_message, "%s has been kicked from the chat.\n", kicked_user.name);
                send_to_chatroom(user.active_chatroom, kick_message, user.id);
                remove_user(kicked_user);
                close(kicked_user.socket_id);
            }
        }
        else if (strcmp(message, "/users") == 0) {
            list_users(user.active_chatroom, socket_id);
        }
        else if (strcmp(message, "/quit") == 0) {
            char quit_message[BUFFER_SIZE];
            sprintf(quit_message, "%s has left the chat.\n", user.name);
            send_to_chatroom(user.active_chatroom, quit_message, user.id);
            remove_user(user);
            close(socket_id);
            pthread_exit(NULL);
        }
        else {
            char formatted_message[BUFFER_SIZE];
            sprintf(formatted_message, "[%s]: %s\n", user.name, message);
            send_to_chatroom(user.active_chatroom, formatted_message, user.id);
        }

        memset(buffer, 0, BUFFER_SIZE); // reset buffer
    }

    return NULL;
}

int main(int argc, char** argv) {
    int server_socket_id = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_id < 0) {
        perror("Could not create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket_id, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Could not bind socket");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket_id, 10) < 0) {
        perror("Could not listen on socket");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Waiting for incoming connections...\n");

        struct sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);
        int client_socket_id = accept(server_socket_id, (struct sockaddr*)&client_address, &client_address_size);

        if (client_socket_id < 0) {
            perror("Could not accept connection");
            exit(EXIT_FAILURE);
        }

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, &client_socket_id);
    }

    close(server_socket_id);
    return 0;
}