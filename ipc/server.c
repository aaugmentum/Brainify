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

//Function prototypes
void terminate(const char *msg);
int generate_luminous_element();
void db_error(MYSQL *conn);
MYSQL_RES *selectQuery(const char *);
int insertQuery(const char *);
void init_socket();
void init_db();
void *handle_client();

//Server structs
typedef struct
{
	pthread_t tid;
	int fd;
	int score;
	char username[20];
} peer_t;

typedef struct
{
	//DB
	game_t game;

	//Players
	peer_t admin;
	peer_t players[8];
	int players_size;

	//Questions
	question_t questions[128];
	int question_size;

	int pin;
} session_t;

//Global variables
int server_fd;
session_t session;
MYSQL *conn;

int main()
{
	init_db();
	init_socket();
	mysql_close(conn);
	return 0;
}

void init_db()
{
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
	else
	{
		fprintf(stdout, "DB connected on %s\n", DB_HOST);
	}
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

			char query[256];
			sprintf(query, SQL_USER_EXIST, auth.username, auth.password);
			MYSQL_RES *sql_result = selectQuery(query);
			if (sql_result->row_count)
			{
				fprintf(stdout, "USERNAME: %s\n", auth.username);
				fprintf(stdout, "PASSWORD: %s\n", auth.password);
				strcpy(peer->username, auth.username);
				result = 1;
			}
			else
				fprintf(stdout, "Wrong credentials...\n");

			sendall(peer->fd, &result, sizeof(int), 0);
		}
		break;
		//Write to database new user
		case SIGNUP:
		{
			auth_t auth;
			memcpy(&auth, method->data, sizeof(auth_t));
			int result = 0;

			char query[256];
			sprintf(query, SQL_USER_CREATE, auth.username, auth.password);

			if (insertQuery(query))
				fprintf(stdout, "Dublicate entry\n");
			else
			{
				fprintf(stdout, "SUCCESS\n");
				fprintf(stdout, "INSERTED into user table\n\tUSERNAME: %s\n\tPASSWORD: %s\n",
						auth.username, auth.password);
				result = 1;
			}

			sendall(peer->fd, &result, sizeof(int), 0);
		}
		break;
		//Client
		case JOIN:
		{
			join_t join;
			memcpy(&join, method->data, sizeof(join_t));

			int result = 0;
			if (join.pin == session.pin && session.players_size && session.pin != 0 < 8)
			{
				session.players[session.players_size] = *peer;
				session.players_size++;
				result = atoi(session.game.game_id);

				fprintf(stdout, "New player connected! %s\n", peer->username);
				sendall(peer->fd, &result, sizeof(int), 0);

				sendall(session.admin.fd, peer->username, sizeof(peer->username), 0);
			}
			else
			{
				fprintf(stdout, "Pin incorrect!\n");
				sendall(peer->fd, &result, sizeof(int), 0);
			}
		}
		break;
		case GET_QUESTIONS:
		{
			char query[256];
			char game_id[8];
			memcpy(game_id, method->data, 8);
			sprintf(query, SQL_QUESTIONS, game_id);
			MYSQL_RES *sql_result = selectQuery(query);

			questions_t questions;
			questions.size = 0;

			MYSQL_ROW row;
			int i = 1, k = 0;
			while ((row = mysql_fetch_row(sql_result)))
			{
				// printf("%s %s %s %s\n", row[0], row[1], row[2], row[3]);
				switch (i)
				{
				case 1:
					strcpy(questions.at[k].question_text, row[1]);
					strcpy(questions.at[k].option1, row[2]);
					if (atoi(row[3]))
						questions.at[k].answer = "1";
					break;
				case 2:
					strcpy(questions.at[k].option2, row[2]);
					if (atoi(row[3]))
						questions.at[k].answer = "2";
					break;
				case 3:
					strcpy(questions.at[k].option3, row[2]);
					if (atoi(row[3]))
						questions.at[k].answer = "3";
					break;
				case 4:
					strcpy(questions.at[k].option4, row[2]);
					if (atoi(row[3]))
						questions.at[k].answer = "4";
					i = 0;
					k++;
					break;
				default:
					break;
				}
				i++;
			}
			questions.size = k;

			mysql_free_result(sql_result);
			sendall(peer->fd, &questions, sizeof(questions_t), 0);
		}
		break;
		case ANSWER:
		{
			int score;
			memcpy(&score, method->data, sizeof(int));
			peer->score += score;
		}
		break;
		//Admin
		case GAMES:
		{
			// char query[256];
			// sprintf(query, SQL_GAMES, peer->username);
			MYSQL_RES *sql_result = selectQuery(SQL_ALL_GAMES);

			games_t games;
			games.size = sql_result->row_count;

			int num_fields = mysql_num_fields(sql_result);
			MYSQL_ROW row;

			int i = 0;
			while ((row = mysql_fetch_row(sql_result)))
			{
				strcpy(games.at[i].game_id, row[0]);
				strcpy(games.at[i].username, row[1]);
				strcpy(games.at[i].title, row[2]);

				game_t game = games.at[i];

				i++;
				printf("GID: %s, Title: %s, Owner: %s\n", game.game_id, game.title, game.username);
			}

			mysql_free_result(sql_result);
			sendall(peer->fd, &games, sizeof(games_t), 0);
		}
		break;
		case START_GAME:
		{
			start_game_t start_game;
			memcpy(&start_game, method->data, sizeof(start_game_t));
			int pin = generate_luminous_element();
			sendall(peer->fd, &pin, sizeof(int), 0);

			strcpy(session.game.game_id, start_game.game_id);
			session.pin = pin;
			session.admin = *peer;
		}
		break;
		case RUN_GAME:
		{
			session.pin = 0;
			for (size_t i = 0; i < session.question_size; i++)
			{
				sleep(20);
			}
		}
		default:
			fprintf(stdout, "Error");
			break;
		}
		fflush(stdout);
	}

	free(method);
	close(peer->fd);
	free(peer);
	pthread_exit(NULL);
}

// void notify_next() {
// 	send()
// }

MYSQL_RES *selectQuery(const char *query)
{
	fprintf(stdout, "%s\n", query);

	if (mysql_query(conn, query))
		db_error(conn);

	MYSQL_RES *result = mysql_store_result(conn);

	if (result == NULL)
		db_error(conn);

	return result;
}

int insertQuery(const char *query)
{
	fprintf(stdout, "%s\n", query);
	return mysql_query(conn, query);
}

void db_error(MYSQL *conn)
{
	fprintf(stderr, "%s\n", mysql_error(conn));
	// mysql_close(conn);
	// exit(1);
}