#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include "headers/utils.h"

void terminate(const char *msg);
void init_socket();

int server_fd, client_fd;
char buf[1024] = {0};
char text[1024] = {0};

int main()
{
	init_socket();
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		fgets(text, sizeof(text), stdin);
		send(client_fd, text, sizeof(text), 0);
	}
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

	//Set socket options
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
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
	client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
	if (client_fd < 0)
		terminate("Socket accept failed");

	puts("Client connected. Type messages:");
}
