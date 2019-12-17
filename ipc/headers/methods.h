#define MAX_DATA_SIZE 65000

typedef enum
{
	POTATO,
	//*Basic
	SIGNUP, 
	SIGNIN, 
	//*Admin
	GAMES,
	START_GAME, 
	RUN_GAME,
	//*Clients
	JOIN,
	GET_QUESTIONS,
	ANSWER
} mtd_type_enum;

typedef enum
{
	STANDINGS,
	PLAYER_JOIN
} standings_type_enum;

//*Send any struct within a struct
typedef struct
{
	int type;
	char data[MAX_DATA_SIZE];
} method_t;

typedef struct
{
	char username[20];
	char password[20];
} auth_t;

typedef struct
{
	char question_text[160];
	char option1[60];
	char option2[60];
	char option3[60];
	char option4[60];
	char answer[2];
} question_t;

typedef struct
{
	question_t at[128];
	int size;
} questions_t;

typedef struct
{
	char game_id[8];
	char title[20];
	char username[20];
} game_t;

typedef struct
{
	game_t at[9];
	int size;
} games_t;

typedef struct
{
	int pin;
} join_t;

typedef struct
{
	char game_id[8];
} start_game_t;

typedef struct
{
	char username[20];
	int score;
} score_t;

typedef struct
{
	score_t at[8];
	int size;
	int type;
	char username[20];
} scores_t;
