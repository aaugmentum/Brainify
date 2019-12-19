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

//*Function prototypes
void terminate(const char *);

//*Basic
int connect_server(char *);
int signup(char *, char *);
int signin(char *, char *);
void signout();

//*Admin
char *games();
int start_game(char *);
void run_game();
char *receive_standings();

//*Client
int join(int);
char *get_questions(char *);
int receiver();
void answer(int score);

//*Global variables
int server_fd;

//!Test
games_t gms();

//!Do not include to .so
int main()
{
	if (connect_server("127.0.0.1") == 0)
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
	// signout();

	// get_questions("208");

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
			char *username = receive_standings();
			printf("Player %d connected: %s\n", ++i, username);
			free(username);
			if (i == 2)
				break;
		}

		sleep(3);
		run_game();
		while (1)
		{
			receive_standings();
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
			while (1)
			{
				int x = receiver();
				printf("%d\n", x);
			}
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

//*Basic
//Return 0 if server is unavailable
int connect_server(char *ip)
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
	if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0)
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
	my_recv(server_fd, &result, sizeof(int), MSG_WAITALL);
	printf("%i", result);
	return result;
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
	my_recv(server_fd, &result, sizeof(int), MSG_WAITALL);
	return result;
}

void signout()
{
	close(server_fd);
}

//*Admin
char *games()
{
	method_t method;
	method.type = GAMES;
	sendall(server_fd, &method, sizeof(method_t), 0);

	//Get list of games of this user
	char *result = malloc(1024);
	memset(result, '\0', 1024);
	games_t games;
	int size = my_recv(server_fd, &games, sizeof(games_t), MSG_WAITALL);
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

int start_game(char *game_id)
{
	method_t method;
	start_game_t start_game;
	strcpy(start_game.game_id, game_id);
	method.type = START_GAME;
	memcpy(method.data, &start_game, sizeof(start_game_t));
	sendall(server_fd, &method, sizeof(method_t), 0);

	int result;
	my_recv(server_fd, &result, sizeof(int), MSG_WAITALL);
	return result;
}

void run_game()
{
	method_t method;
	method.type = RUN_GAME;
	sendall(server_fd, &method, sizeof(method), 0);
}

char *receive_standings()
{
	scores_t scores;
	my_recv(server_fd, &scores, sizeof(scores_t), MSG_WAITALL);

	const int length = 512;
	char *buf = malloc(length);
	memset(buf, '\0', length);

	if (scores.type == PLAYER_JOIN)
	{
		strcpy(buf, scores.username);
	}
	else
	{
		for (size_t i = 0; i < scores.size; i++)
		{
			char temp[32] = {"\0"};
			sprintf(temp, "%s: %d,", scores.at[i].username, scores.at[i].score);
			strcat(buf, temp);
			printf("%s: %d\n", scores.at[i].username, scores.at[i].score);
		}
	}

	return buf;
}

//*Client
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
	my_recv(server_fd, &result, sizeof(int), MSG_WAITALL);
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
	my_recv(server_fd, &questions, sizeof(questions_t), MSG_WAITALL);
	printf("%d\n", questions.size);

	const int length = 60000;
	char *buf = malloc(length);
	memset(buf, '\0', length);
	for (size_t i = 0; i < questions.size; i++)
	{
		question_t question = questions.at[i];
		strcat(buf, question.question_text);
		strcat(buf, "^");
		strcat(buf, question.option1);
		strcat(buf, "@");
		strcat(buf, question.option2);
		strcat(buf, "@");
		strcat(buf, question.option3);
		strcat(buf, "@");
		strcat(buf, question.option4);
		strcat(buf, "^");
		strcat(buf, question.answer);
		strcat(buf, "$");
		printf("Question: %s\n1: %s\n2: %s\n3: %s\n4: %s\nAnswer: %s\n", question.question_text, question.option1, question.option2, question.option3, question.option4, question.answer);
	}
	return buf;
}

int receiver()
{
	int result;
	my_recv(server_fd, &result, sizeof(int), MSG_WAITALL);
	return result;
}

void answer(int score)
{
	method_t method;
	method.type = ANSWER;
	memcpy(method.data, &score, sizeof(int));
	sendall(server_fd, &method, sizeof(method_t), MSG_WAITALL);
}

//!Test
games_t gms()
{
	method_t method;
	method.type = GAMES;
	sendall(server_fd, &method, sizeof(method_t), 0);

	//Get list of games of this user
	games_t games;
	my_recv(server_fd, &games, sizeof(games_t), MSG_WAITALL);

	return games;
}