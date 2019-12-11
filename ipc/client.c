
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
#include "headers/methods.h"

void terminate(const char *msg);
int connect_server();
char *get_msg();

int server_fd;
char buf[1024] = {0};

int main()
{
	if (connect_server() == 0)
		exit(EXIT_FAILURE);

	size_t method_size = sizeof(struct method);
	struct method *mtd = malloc(method_size);
	size_t login_size = sizeof(struct login);
	struct login *lgn = malloc(login_size);

	recv(server_fd, mtd, method_size, 0);
	memcpy(lgn, mtd->data, login_size);
	printf("Type: %d\nMsg: %s\nLogin: %d\n", mtd->type, mtd->msg, lgn->x);

	free(mtd);

	return 0;
}

//Return 0 if server is unavailable
int connect_server()
{
	//Init variables
	int is_connect = 1;
	struct sockaddr_in server_addr;
	int addrlen = sizeof(server_addr);

	//Init socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		perror("Socket failed");
		return 0;
	}

	//Connect to server
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0)
	{
		perror("Address cant't be converted");
		return 0;
	}
	if (connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("Connection failed");
		return 0;
	}

	return 1;
}

char *get_msg()
{
	memset(buf, 0, sizeof(buf));
	recv(server_fd, buf, sizeof(buf), 0);
	printf("%s", buf);
	return buf;
}