#define MAX_DATA_SIZE 65000

typedef enum
{
	POTATO,
	//User
	SIGNUP,
	SIGNIN,
	LOGOUT,
	//Game
	CREATE,
	START_GAME,
	JOIN,
	MY_GAMES,
	EDIT,
	DELETE,
	FINISH,
	//In game
	ANSWER,
	RESULT
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
	char title[160];
	char option1[32];
	char option2[32];
	char option3[32];
	char option4[32];
	int answer;
} question_t;

typedef struct
{
	question_t questions[128];
	int size;
} create_t;

typedef struct
{
	int pin;
} join_t;

typedef struct
{
	//0 Error, 1 Success
	int status;
} result_t;

typedef struct
{
	int gid;
} start_game_t;