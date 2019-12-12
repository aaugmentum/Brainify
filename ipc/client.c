
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
int login(const char[16], const char[128]);
char *get_msg();

int server_fd;
char buf[1024] = {0};
question_t questions[128];
int questions_size;

int main()
{
	if (connect_server() == 0)
		exit(EXIT_FAILURE);

	login("aaugmentum", "12354");
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

int login(const char username[16], const char password[128]) {
	method_t *method = malloc(sizeof(method_t));
	method->type = LOGIN;
	auth_t *auth = malloc(sizeof(sizeof(auth)));
	memcpy(auth->username, username, 16);
	memcpy(auth->password, password, 128);
	memcpy(method->data, auth, sizeof(auth_t));

	send(server_fd, method, sizeof(method_t), 0);
	free(method);
	free(auth);
}

int signup(const char username[16], const char password[128]) {
	method_t *method = malloc(sizeof(method_t));
	method->type = SIGNUP;
	auth_t *auth = malloc(sizeof(sizeof(auth)));
	memcpy(auth->username, username, 16);
	memcpy(auth->password, password, 128);
	memcpy(method->data, auth, sizeof(auth_t));

	send(server_fd, method, sizeof(method_t), 0);
	free(method);
	free(auth);
}

int logout(){
	method_t *method = malloc(sizeof(method_t));
	method->type = LOGOUT;
	send(server_fd, method, sizeof(method_t), 0);
	free(method);
}

// int create_question() {
// 	questions_size = 0;
// 	memset(&questions, 0, sizeof(question_t[128]));
// }

// void add_question() {

// 	questions[questions_size].answer = ;
// 	questions[questions_size].title = ;

// 	questions_size++;
// }

// void finish_question() {
	
// }

char *get_msg()
{
	memset(buf, 0, sizeof(buf));
	recv(server_fd, buf, sizeof(buf), 0);
	printf("%s", buf);
	return buf;
}