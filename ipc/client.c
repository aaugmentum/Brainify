#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include "headers/utils.h"

#define PORT 8080

void terminate(const char *msg);
void InitSocket();

int server_fd;
char buf[1024] = {0};

// int main()
// {
// 	init_socket();
// 	while (1)
// 	{
// 		memset(buf, 0, sizeof(buf));
// 		recv(server_fd, buf, sizeof(buf), 0);
// 		printf("%s", buf);
// 	}
// 	return 0;
// }

void InitSocket()
{
	//Init variables
	struct sockaddr_in server_addr;
	int addrlen = sizeof(server_addr);

	//Init socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	//Connect to server
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0)
		terminate("Address is not supported");

	if (connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0);
		// terminate("Socket connect failed");
}

char *GetMsg()
{
	memset(buf, 0, sizeof(buf));
	recv(server_fd, buf, sizeof(buf), 0);
	printf("%s", buf);
	return buf;
}