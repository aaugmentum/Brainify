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
list_t clients;
char buf[1024] = {0};
char text[1024] = {0};

int main()
{
	init_socket();
	return 0;
}

void init_socket()
{
	clients.size = 0;
	//Init variables
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	int opt = 1;

	//Init socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
		terminate("Socket failed");

	//Set socket options
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
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
		int *client_fd = malloc(sizeof(int));
		*client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
		if (client_fd < 0)
			perror("Socket accept failed");

		// puts("New client connected. Type:");
		clients.at[clients.size++] = *client_fd;
		pthread_t tid;
		pthread_create(&tid, NULL, handle_client, client_fd);
	}
}

void *handle_client(int *client_fd)
{
	short finish = 0;
	char buf[sizeof(method_t)];
	int count = 0;
	while (1)
	{
		method_t *method = malloc(sizeof(method_t));
		memset(buf, 0, sizeof(method_t));
		int size = recv(*client_fd, buf, sizeof(method_t), 0);
		memcpy(method, buf, sizeof(method_t));
		if (size > 0)
		{
			// printf("Type: %d, Size: %d\n", method->type, size);

			switch (method->type)
			{
			//Check if user is registred
			case LOGIN:
			{
				auth_t *auth = malloc(sizeof(auth_t));
				memcpy(auth, method->data, sizeof(auth_t));

				count++;
				printf("%d\n", count);

				// printf("USERNAME: %s\n", auth->username);
				// printf("PASSWORD: %s\n", auth->password);
				free(auth);
			}
			break;
			//Write to database new user
			case SIGNUP:
			{
				auth_t *auth = malloc(sizeof(auth_t));
				memcpy(auth, method->data, sizeof(auth_t));

				// printf("USERNAME: %s\n", auth->username);
				// printf("PASSWORD: %s\n", auth->password);
				free(auth);
			}
			break;
			//Delete from list and join thread
			case LOGOUT:
			{
				printf("LOGOUT");
				finish = 1;
			}
			break;
			default:
				break;
			}
		}
		free(method);
	}
}