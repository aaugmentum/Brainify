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

int server_fd, client_fd;
char buf[1024] = {0};
char text[1024] = {0};

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
		client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
		if (client_fd < 0)
			perror("Socket accept failed");

		// puts("New client connected. Type:");
		pthread_t tid;
		pthread_create(&tid, NULL, handle_client, NULL);
	}
}

void *handle_client()
{
	size_t method_size = sizeof(struct method);
	struct method *mtd = malloc(method_size);
	size_t login_size = sizeof(struct login);
	struct login *lgn = malloc(login_size);

	mtd->type = 1;
	lgn->x = 55;
	memcpy(mtd->data, lgn, login_size);
	strcpy(mtd->msg, "Hello");

	send(client_fd, mtd, method_size, 0);
	free(mtd);
	// while (1)
	// {
	// 	memset(buf, 0, sizeof(buf));
	// 	fgets(text, sizeof(text), stdin);
	// 	send(client_fd, text, sizeof(text), 0);
	// }
}