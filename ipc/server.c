#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include "headers/utils.h"
#include "headers/methods.h"

void terminate(const char *msg);
void init_socket();
void *handle_client();

int server_fd;
typedef struct
{
	pthread_t tid;
	int fd;
} peer_t;

int main()
{
	init_socket();
	return 0;
}

void init_socket()
{
	//Init variables
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	int opt = 1;

	//Init socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
		terminate("Socket failed");

	//Set socket options
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		terminate("Socket setopt failed");

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	//Bind socket to port
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		terminate("Socket bind failed");

	//Listen port
	printf("Listening on port %d...\n", PORT);
	if (listen(server_fd, 10) < 0)
		terminate("Socket listen failed");

	//Accept clients
	while (1)
	{
		peer_t *peer = malloc(sizeof(peer_t));
		peer->fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
		if (peer->fd < 0)
			perror("Socket accept failed");

		// puts("New client connected. Type:");
		pthread_t tid;
		pthread_create(&tid, NULL, handle_client, peer);
	}
}

void *handle_client(peer_t *peer)
{
	peer->tid = pthread_self();
	int count = 0;
	while (1)
	{
		method_t *method = malloc(sizeof(method_t));
		int size = recv(peer->fd, method, sizeof(method_t), MSG_WAITALL);

		//Client disconnected
		if (size < 1)
		{
			fprintf(stdout, "Client disconnected\n");
			free(method);
			break;
		}

		switch (method->type)
		{
		//Check if user is registred
		case LOGIN:
		{
			auth_t *auth = malloc(sizeof(auth_t));
			memcpy(auth, method->data, sizeof(auth_t));

			count++;
			printf("USERNAME: %s\n", auth->username);
			printf("PASSWORD: %s\n", auth->password);
			free(auth);
		}
		break;
		//Write to database new user
		case SIGNUP:
		{
			auth_t *auth = malloc(sizeof(auth_t));
			memcpy(auth, method->data, sizeof(auth_t));

			printf("USERNAME: %s\n", auth->username);
			printf("PASSWORD: %s\n", auth->password);
			free(auth);
		}
		break;
		default:
			break;
		}

		fflush(stdout);
		free(method);
	}

	printf("Count: %d\n", count);
	close(peer->fd);
	free(peer);
	pthread_exit(NULL);
}