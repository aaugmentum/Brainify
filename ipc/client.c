
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
// #include "headers/methods.h"

void terminate(const char *msg);
int connect_server(void);
int signin(char *, char *);
int logout(void);
int join(int);
int start_game(int);

int server_fd;
question_t questions[128];
int questions_size;

int main()
{
	if (connect_server() == 0)
		exit(EXIT_FAILURE);

	signin("aaugmentum", "12354");

	int x;
	printf("Enter what you want (0 to start): ");
	scanf("%d", &x);

	if(x == 0){
		int pin = start_game(12345);
		printf("PIN for the game: %d", pin);
	}else{
		printf("Enter game pin: ");
		scanf("%d", &x);
		join(x);
	}
	close(server_fd);
	return 0;
}

//Return 0 if server is unavailable
int connect_server()
{
	//Init variables
	struct sockaddr_in server_addr;

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

int sendall(int fd, method_t *buf, int n, int flags)
{
	int total = 0, temp;

	while (total < n)
	{
		temp = send(fd, buf + total, n - total, flags);
		if (temp == -1)
			break;
		total += temp;
	}

	return (temp == -1 ? -1 : total);
}

int signin(char *username, char *password)
{
	method_t *method = malloc(sizeof(method_t));
	method->type = SIGNIN;
	auth_t *auth = malloc(sizeof(auth_t));
	memcpy(auth->username, username, 16);
	memcpy(auth->password, password, 128);
	memcpy(method->data, auth, sizeof(auth_t));

	sendall(server_fd, method, sizeof(method_t), 0);

	int result;
	recv(server_fd, &result, sizeof(int), MSG_WAITALL);

	printf("Result: %d\n", result);
	free(method);
	free(auth);

	return result;
}

int signup(const char username[16], const char password[128])
{
	method_t *method = malloc(sizeof(method_t));
	method->type = SIGNUP;
	auth_t *auth = malloc(sizeof(auth_t));
	memcpy(auth->username, username, 16);
	memcpy(auth->password, password, 128);
	memcpy(method->data, auth, sizeof(auth_t));

	sendall(server_fd, method, sizeof(method_t), 0);
	free(method);
	free(auth);

	return 1;
}

int logout()
{
	method_t *method = malloc(sizeof(method_t));
	method->type = LOGOUT;
	sendall(server_fd, method, sizeof(method_t), 0);

	free(method);

	return 1;
}

int join(int pin){
	method_t method;
	join_t join;
	method.type = JOIN;
	join.pin = pin;
	memcpy(method.data, &join, sizeof(join_t));
	sendall(server_fd, &method, sizeof(method_t), 0);
	int result;
	recv(server_fd, &result, sizeof(int), MSG_WAITALL);
	return result;
}

int wait_for_game(){
	int result;
	recv(server_fd, &result, sizeof(int), MSG_WAITALL);
	return result;
}

int start_game(int gid){
	method_t method;
	start_game_t start_game;
	start_game.gid = gid;
	method.type = START_GAME;
	memcpy(method.data, &start_game, sizeof(start_game_t));
	sendall(server_fd, &method, sizeof(method_t), 0);

	int result;
	recv(server_fd, &result, sizeof(int), MSG_WAITALL);
	return result;
}