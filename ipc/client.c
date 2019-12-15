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

//Function prototypes
void terminate(const char *);
int signin(char *, char *);
int signup(char *, char *);
char *games();
int join(int);
int start_game(char *);
int connect_server();
void sign();
void potato();
games_t gms();
char *player_join();
char *get_questions(char *);
int receiver();

//Global variables
int server_fd;

int main()
{
	if (connect_server() == 0)
		exit(EXIT_FAILURE);

	int temp;
A:
	printf("Sign in\n");
	char username[20], password[20];
	printf("Enter username: ");
	scanf("%s", username);
	printf("Enter password: ");
	scanf("%s", password);

	temp = signin(username, password);
	if (temp)
		printf("Signed In\n");
	else
	{
		system("clear");
		goto A;
	}

	get_questions("208");

	printf("Create/Join game(0/1): ");
	scanf("%d", &temp);
	if (!temp)
	{
		printf("Games?\n");
		games_t games = gms();
		for (size_t i = 0; i < games.size; i++)
		{
			game_t game = games.at[i];
			printf("%ld: Title: %s\n", i + 1, game.title);
		}
		printf("Which game to start: ");
		scanf("%d", &temp);
		temp = start_game(games.at[temp - 1].game_id);
		system("clear");
		printf("Game pin is: %d\n", temp);

		int i = 0;
		while (1)
		{
			char *username = player_join();
			printf("Player %d connected: %s\n", ++i, username);
			free(username);
			// printf("Do you want to start game?(1/0) ");
			// scanf("%d", &temp);
			if (temp)
				break;
		}

		scanf("%d", &temp);
	}
	else
	{
	B:
		printf("Enter game pin: ");
		scanf("%d", &temp);
		temp = join(temp);
		if (temp)
		{
			printf("You joined to the game!\n");
		}
		else
		{
			system("clear");
			goto B;
		}
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

int signin(char *username, char *password)
{
	method_t method;
	auth_t auth;
	method.type = SIGNIN;
	memcpy(auth.username, username, sizeof(auth.username));
	memcpy(auth.password, password, sizeof(auth.password));
	memcpy(method.data, &auth, sizeof(auth_t));
	sendall(server_fd, &method, sizeof(method_t), 0);

	//0 Wrong credentials, 1 Success
	int result;
	recv(server_fd, &result, sizeof(int), MSG_WAITALL);
	return result;
}

int signup(char *username, char *password)
{
	method_t method;
	auth_t auth;
	method.type = SIGNUP;
	memcpy(auth.username, username, sizeof(auth.username));
	memcpy(auth.password, password, sizeof(auth.password));
	memcpy(method.data, &auth, sizeof(auth_t));
	sendall(server_fd, &method, sizeof(method_t), 0);

	//0 Already exist, 1 Success
	int result;
	recv(server_fd, &result, sizeof(int), MSG_WAITALL);
	printf("%i", result);
	return result;
}

char *games()
{
	method_t method;
	method.type = GAMES;
	sendall(server_fd, &method, sizeof(method_t), 0);

	//Get list of games of this user
	char *result = malloc(1024);
	memset(result, '\0', 1024);
	games_t games;
	int size = recv(server_fd, &games, sizeof(games_t), MSG_WAITALL);
	for (size_t i = 0; i < games.size; i++)
	{
		game_t game = games.at[i];

		strcat(result, game.title);
		strcat(result, "/");
		strcat(result, game.game_id);
		strcat(result, ",");

		printf("GID: %s, Title: %s, Owner: %s\n", game.game_id, game.title, game.username);
	}
	printf("%s", result);
	return result;
}

games_t gms()
{
	method_t method;
	method.type = GAMES;
	sendall(server_fd, &method, sizeof(method_t), 0);

	//Get list of games of this user
	games_t games;
	recv(server_fd, &games, sizeof(games_t), MSG_WAITALL);

	return games;
}

char *player_join()
{
	char *username = malloc(20);
	memset(username, '\0', 20);
	recv(server_fd, username, 20, MSG_WAITALL);
	return username;
}

void signout()
{
	// method_t method;
	// method.type = LOGOUT;
	// sendall(server_fd, &method, sizeof(method_t), 0);
	close(server_fd);
}

//Return 0 if pin is incorrect or game_id
int join(int pin)
{
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

int receiver()
{
	int result;
	recv(server_fd, &result, sizeof(int), MSG_WAITALL);
	return result;
}

int start_game(char *game_id)
{
	method_t method;
	start_game_t start_game;
	strcpy(start_game.game_id, game_id);
	method.type = START_GAME;
	memcpy(method.data, &start_game, sizeof(start_game_t));
	sendall(server_fd, &method, sizeof(method_t), 0);

	int result;
	recv(server_fd, &result, sizeof(int), MSG_WAITALL);
	return result;
}

char *get_questions(char *game_id)
{
	method_t method;
	method.type = GET_QUESTIONS;
	memset(method.data, '\0', sizeof(65000));
	strcpy(method.data, game_id);
	sendall(server_fd, &method, sizeof(method), 0);

	questions_t questions;
	recv(server_fd, &questions, sizeof(questions_t), MSG_WAITALL);
	printf("%d\n", questions.size);

	const int length = 60000;
	char *buf = malloc(length);
	memset(buf, '\0', length);
	for (size_t i = 0; i < questions.size; i++)
	{
		question_t question = questions.at[i];
		strcat(buf, question.question_text);
		strcat(buf, "^^");
		strcat(buf, question.option1);
		strcat(buf, "||");
		strcat(buf, question.option1);
		strcat(buf, "||");
		strcat(buf, question.option2);
		strcat(buf, "||");
		strcat(buf, question.option3);
		strcat(buf, "||");
		strcat(buf, question.option4);
		strcat(buf, "@@");
		strcat(buf, question.answer);
		strcat(buf, "$$");
		printf("Question: %s\n1: %s\n2: %s\n3: %s\n4: %s\nAnswer: %s\n", question.question_text, question.option1, question.option2, question.option3, question.option4, question.answer);
	}
	return buf;
}
