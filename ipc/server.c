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

#define MAX_PLAYERS 8
//Function prototypes
void terminate(const char *);
int generate_luminous_element();
void db_error(MYSQL *);
MYSQL_RES *selectQuery(const char *);
int insertQuery(const char *);
void init_socket();
void init_db();
void *handle_client();
void notify_next();
void standings();
int comp_score(const void *, const void *);

//Server structs
typedef struct
{
	pthread_t tid;
	int fd;
	int score;
	char username[20];
} peer_t;

typedef enum
{
	INITIAL,
	LOBBY,
	INGAME,
} session_enum;

typedef struct
{
	//DB
	game_t game;

	//Players
	peer_t admin;
	peer_t *players[MAX_PLAYERS];
	int players_size;

	//Questions
	question_t questions[128];
	int question_size;

	int pin;
	int state;
} session_t;

//Global variables
int server_fd;
session_t session;
MYSQL *conn;

int main()
{
	srand(time(NULL));
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
		perror("Could not init DB");
		return;
	}
	if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASSWD, DB_DB, DB_PORT, NULL, 0) == NULL)
	{
		perror("DB Connection Error");
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
	fprintf(stdout, "Listening on port %d...\n\n", PORT);
	if (listen(server_fd, 10) < 0)
		terminate("Socket listen failed");

	//Accept clients
	while (1)
	{
		peer_t *peer = malloc(sizeof(peer_t));
		peer->fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
		// peer->score = 0;
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
		//!Client disconnected
		if (size < 1)
		{
			fprintf(stdout, "Client %s disconnected\n", peer->username);
			break;
		}

		switch (method->type)
		{
		//*Basic
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
				fprintf(stdout, "SIGNIN: User %s is signed!\n", auth.username);
				strcpy(peer->username, auth.username);
				result = 1;
			}
			else
				fprintf(stdout, "SIGNIN: Wrong credentials...\n");

			mysql_free_result(sql_result);
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
				fprintf(stdout, "SIGNUP: User is already exists!\n");
			else
			{
				fprintf(stdout, "SIGNUP: User %s added to databse\n", auth.username);
				result = 1;
			}

			sendall(peer->fd, &result, sizeof(int), 0);
		}
		break;
		//*Client
		case JOIN:
		{
			join_t join;
			memcpy(&join, method->data, sizeof(join_t));

			int result = 0;
			if (join.pin == session.pin && session.players_size < 8 && session.state == LOBBY)
			{
				session.players[session.players_size] = peer;
				session.players_size++;
				result = atoi(session.game.game_id);

				fprintf(stdout, "JOIN: Player %s connected!\n", peer->username);
				sendall(peer->fd, &result, sizeof(int), 0);

				sendall(session.admin.fd, peer->username, sizeof(peer->username), 0);
				int x = generate_luminous_element();
				printf("%d", x);
				peer->score = x;
			}
			else
			{
				fprintf(stdout, "JOIN: Pin incorrect!\n");
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
			fprintf(stdout, "QUESTION: Getting questions for %s game\n", game_id);
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
						strcpy(questions.at[k].answer, "0");
					break;
				case 2:
					strcpy(questions.at[k].option2, row[2]);
					if (atoi(row[3]))
						strcpy(questions.at[k].answer, "1");
					break;
				case 3:
					strcpy(questions.at[k].option3, row[2]);
					if (atoi(row[3]))
						strcpy(questions.at[k].answer, "2");
					break;
				case 4:
					strcpy(questions.at[k].option4, row[2]);
					if (atoi(row[3]))
						strcpy(questions.at[k].answer, "3");
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
			if (score < 0)
				fprintf(stdout, "ANSWER: User %s answered incorrect!", peer->username);
			else
				fprintf(stdout, "ANSWER: User %s answered correct!", peer->username);

			memcpy(&score, method->data, sizeof(int));
			peer->score += score;
		}
		break;
		//*Admin
		case GAMES:
		{
			fprintf(stdout, "GAMES: Getting all games\n");
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
			}
			mysql_free_result(sql_result);
			sendall(peer->fd, &games, sizeof(games_t), 0);
		}
		break;
		case START_GAME:
		{
			if (session.state == INITIAL)
			{
				session.state = LOBBY;
				start_game_t start_game;
				memcpy(&start_game, method->data, sizeof(start_game_t));
				int pin = generate_luminous_element();
				sendall(peer->fd, &pin, sizeof(int), 0);

				strcpy(session.game.game_id, start_game.game_id);
				session.pin = pin;
				session.admin = *peer;

				char query[256];
				sprintf(query, SQL_QUESTIONS_COUNT, session.game.game_id);
				MYSQL_RES *sql_result = selectQuery(query);
				MYSQL_ROW row = mysql_fetch_row(sql_result);
				session.question_size = atoi(row[0]);
				printf("START_GAME: User %s created a lobby with pin %d\n", peer->username, pin);
			}
			else
			{
				int err = 0;
				sendall(peer->fd, &err, sizeof(int), 0);
				printf("START_GAME: Wait for finish\n");
			}
		}
		break;
		case RUN_GAME:
		{
			session.state = INGAME;
			notify_next(1);
			for (size_t i = 0; i < session.question_size - 1; i++)
			{
				sleep(5);
				notify_next(2);

				//*Standings
				standings();
			}
			sleep(5);
			notify_next(3);
			standings();

			//*Clear session
			session.players_size = 0;
			session.question_size = 0;
			memset(session.players, 0, sizeof(peer_t[MAX_PLAYERS]));
			session.state = INITIAL;
		}
		break;
		default:
			fprintf(stdout, "NO SUCH METHOD\n");
			break;
		}
		fflush(stdout);
	}

	//!Free memory and finish thread
	free(method);
	close(peer->fd);
	free(peer);
	pthread_exit(NULL);
}

void standings()
{
	peer_t players[8];
	for (size_t i = 0; i < session.players_size; i++)
		players[i] = *session.players[i];

	qsort(players, session.players_size, sizeof(peer_t), comp_score);

	//Send
	scores_t scores;
	scores.size = session.players_size;
	for (size_t i = 0; i < session.players_size; i++)
	{
		strcpy(scores.at[i].username, players[i].username);
		scores.at[i].score = players[i].score;
	}
	sendall(session.admin.fd, &scores, sizeof(scores_t), 0);
}

//*Comparator for peers
int comp_score(const void *elem1, const void *elem2)
{
	peer_t f = *((peer_t *)elem1);
	peer_t s = *((peer_t *)elem2);
	if (f.score < s.score)
		return 1;
	if (f.score > s.score)
		return -1;
	return 0;
}

void notify_next(int code)
{
	for (size_t i = 0; i < session.players_size; i++)
	{
		sendall(session.players[i]->fd, &code, sizeof(int), 0);
	}
}

MYSQL_RES *selectQuery(const char *query)
{
	fprintf(stdout, "SQL QUERY: %s\n", query);

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
	perror(mysql_error(conn));
}