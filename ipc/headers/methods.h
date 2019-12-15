#define MAX_DATA_SIZE 65000

typedef enum
{
	POTATO,
	//User
	SIGNUP,
	SIGNIN,
	LOGOUT,
	//Game
	// CREATE,
	START_GAME,
	JOIN,
	GAMES,
	GET_QUESTIONS,
	// EDIT,
	// DELETE,
	FINISH,
	//In game
	RUN_GAME,
	ANSWER,
	RESULT,
} mtd_type_enum;

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
	int status;
	
} join_result_t;

typedef struct
{
	//0 Error, 1 Success
	int status;
} result_t;

typedef struct
{
	char game_id[8];
} start_game_t;