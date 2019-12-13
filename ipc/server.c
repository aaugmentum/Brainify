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
#include <time.h>
#include "headers/utils.h"
// #include "headers/methods.h"

void terminate(const char *msg);
void init_socket();
void *handle_client();
int generate_pin();


typedef struct
{
	pthread_t tid;
	int fd;
	int score;
} peer_t;

typedef struct server
{
	int gid;
	int pin;
	peer_t players[8];
	peer_t admin;
	int size;
} game_t;
int server_fd;
game_t game;

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

void *handle_client(peer_t *peer)
{
	method_t *method = malloc(sizeof(method_t));
	peer->tid = pthread_self();
	while (1)
	{
		memset(method, 0, sizeof(method_t));
		int size = recv(peer->fd, method, sizeof(method_t), MSG_WAITALL);
		// fprintf(stdout, "Received: %d bytes\n", size);

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
			auth_t *auth = malloc(sizeof(auth_t));
			memcpy(auth, method->data, sizeof(auth_t));
			int result = 0;
			if (!strcmp(auth->username, "aaugmentum") && !strcmp(auth->password, "12354"))
			{
				printf("USERNAME: %s\n", auth->username);
				printf("PASSWORD: %s\n", auth->password);
				result = 1;
			}
			
			send(peer->fd, &result, sizeof(int), 0);
			free(auth);
		}
		break;
		//Write to database new user
		case SIGNUP:
		{
			auth_t *auth = malloc(sizeof(auth_t));
			memcpy(auth, method->data, sizeof(auth_t));

			printf("USERNAME: %s\n", auth->username);
			printf("PASSWORD: %s\n", auth->password);
			free(auth);
		}
		break;
		case JOIN:
		{
			join_t *join = malloc(sizeof(join_t));
			memcpy(join, method->data, sizeof(join_t));
			int result = 0;
			if(join->pin == game.pin && game.size < 8){
				game.players[game.size] = *peer;
				game.size++;
				result = 1;

				printf("New player connected!\n");
			}else{
				printf("Pin incorrect!\n");
			}
			send(peer->fd, &result, sizeof(int), 0);

		}
		break;
		case START_GAME:
		{
			start_game_t start_game;
			memcpy(&start_game, method->data, sizeof(start_game_t));
			int pin = generate_pin();
			game.gid = start_game.gid;
			game.pin = pin;
			send(peer->fd, &pin, sizeof(int), 0);
		}
		break;
		default:
			break;
		}

		fflush(stdout);
	}

	free(method);
	close(peer->fd);
	free(peer);
	pthread_exit(NULL);

}


int generate_pin(){
	srand(time(NULL));   // Initialization, should only be called once.
	int r = rand()%100000;  
	return r;
}