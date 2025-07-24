//GEMINI-pro DATASET v1.0 Category: Automated Fortune Teller ; Style: asynchronous
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

static int server_socket;

static void *handle_client(void *arg) {
	int client_socket = *(int *)arg;
	char buffer[256];
	int bytes_read;

	while ((bytes_read = read(client_socket, buffer, sizeof(buffer))) > 0) {
		int fortune = rand() % 10;
		char *fortunes[] = {
			"You will have a great day!",
			"You will meet a new friend.",
			"You will get a promotion.",
			"You will win a lottery.",
			"You will find love.",
			"You will have a successful year.",
			"You will be happy and healthy.",
			"You will achieve your goals.",
			"You will have a long and prosperous life.",
			"You will be loved and admired."
		};

		int fortune_length = strlen(fortunes[fortune]);
		send(client_socket, fortunes[fortune], fortune_length, 0);
	}

	close(client_socket);
	return NULL;
}

static void signal_handler(int signum) {
	close(server_socket);
	exit(0);
}

int main() {
	srand(time(NULL));

	signal(SIGINT, signal_handler);

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		perror("socket");
		exit(1);
	}

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(PORT);

	if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
		perror("bind");
		exit(1);
	}

	if (listen(server_socket, 10) == -1) {
		perror("listen");
		exit(1);
	}

	while (1) {
		int client_socket = accept(server_socket, NULL, NULL);
		if (client_socket == -1) {
			perror("accept");
			continue;
		}

		pthread_t thread;
		if (pthread_create(&thread, NULL, handle_client, &client_socket) != 0) {
			perror("pthread_create");
			close(client_socket);
			continue;
		}
	}

	close(server_socket);

	return 0;
}