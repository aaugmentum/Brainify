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
#include <mysql/mysql.h>
#include "headers/utils.h"
#include "headers/consts.h"

//Function prototypes
void terminate(const char *msg);
void init_socket();
int generate_luminous_element();
void *handle_client();
void finish_with_error(MYSQL *conn);

//Server structs
typedef struct
{
	pthread_t tid;
	int fd;
	int score;
	char username[16];
} peer_t;

typedef struct server
{
	int gid;
	int pin;
	peer_t players[8];
	peer_t admin;
	int size;
} game_t;

//Global variables
int server_fd;
game_t game;
MYSQL *conn;

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

	//MySQL init
	if ((conn = mysql_init(NULL)) == NULL)
	{
		fprintf(stderr, "Could not init DB\n");
		return;
	}
	if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASSWD, DB_DB, DB_PORT, NULL, 0) == NULL)
	{
		fprintf(stderr, "DB Connection Error\n");
		return;
	}

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
		peer->score = 0;
		if (peer->fd < 0)
			perror("Socket accept failed");

		// puts("New client connected. Type:");
		pthread_t tid;
		pthread_create(&tid, NULL, handle_client, peer);
	}
}

void *handle_client(peer_t *peer)
{
	method_t *method = malloc(sizeof(method_t));
	peer->tid = pthread_self();
	while (1)
	{
		memset(method, 0, sizeof(method_t));
		int size = recv(peer->fd, method, sizeof(method_t), MSG_WAITALL);
		//Client disconnected
		if (size < 1)
		{
			fprintf(stdout, "Client disconnected\n");
			break;
		}

		switch (method->type)
		{
		//Check if user is registred
		case SIGNIN:
		{
			auth_t auth;
			memcpy(&auth, method->data, sizeof(auth_t));
			int result = 0;
			if (!strcmp(auth.username, "aaugmentum") && !strcmp(auth.password, "12354"))
			{
				fprintf("USERNAME: %s\n", auth.username);
				fprintf("PASSWORD: %s\n", auth.password);
				result = 1;
			}
			else
			{
				fprintf(stdout, "Wrong credentials...");
			}

			sendall(peer->fd, &result, sizeof(int), 0);
		}
		break;
		//Write to database new user
		case SIGNUP:
		{
			auth_t auth;
			memcpy(&auth, method->data, sizeof(auth_t));

			fprintf(stdout, "USERNAME: %s\n", auth.username);
			fprintf(stdout, "PASSWORD: %s\n", auth.password);
		}
		break;
		case JOIN:
		{
			join_t join;
			memcpy(&join, method->data, sizeof(join_t));
			int result = 0;
			if (join.pin == game.pin && game.size < 8)
			{
				game.players[game.size] = *peer;
				game.size++;
				result = 1;

				fprintf(stdout, "New player connected!\n");
			}
			else
			{
				fprintf(stdout, "Pin incorrect!\n");
			}
			sendall(peer->fd, &result, sizeof(int), 0);
		}
		break;
		case START_GAME:
		{
			start_game_t start_game;
			memcpy(&start_game, method->data, sizeof(start_game_t));
			int pin = generate_luminous_element();
			game.gid = start_game.gid;
			game.pin = pin;
			sendall(peer->fd, &pin, sizeof(int), 0);
		}
		break;
		//Debugging DB connection
		case POTATO:
		{
			if (mysql_query(conn, "select question_text, answer_text from question natural join answer where is_correct = true")) 
			{
				finish_with_error(conn);
			}

			MYSQL_RES *result = mysql_store_result(conn);

			if (result == NULL) 
			{
				finish_with_error(conn);
			}

			int num_fields = mysql_num_fields(result);

			MYSQL_ROW row;

			while ((row = mysql_fetch_row(result))) 
			{ 
				for(int i = 0; i < num_fields; i++) 
				{ 
					printf("%s ", row[i] ? row[i] : "NULL"); 
				} 
				printf("\n"); 
			}
			mysql_free_result(result);
			int test = 1;
			send(peer->fd, &test, sizeof(int), 0);
		}
		break;
		default:
			fprintf(stdout, "Error");
			break;
		}
		fflush(stdout);
	}

  	mysql_close(conn);
	free(method);
	close(peer->fd);
	free(peer);
	pthread_exit(NULL);
}

void finish_with_error(MYSQL *conn)
{
  fprintf(stderr, "%s\n", mysql_error(conn));
  mysql_close(conn);
  exit(1);        
}