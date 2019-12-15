#define REMOTE 0
#if REMOTE
	#define DB_HOST "remotemysql.com"
	#define DB_USER "71VUvOSzmm"
	#define DB_PASSWD "rpraRefD0g"
	#define DB_DB "71VUvOSzmm"
#else
	#define DB_HOST "localhost"
	#define DB_USER "azamat"
	#define DB_PASSWD "12354"
	#define DB_DB "brainify"
#endif

#define DB_PORT 3306
#define PORT 8080

//SQL queries 
#define SQL_USER_EXIST "select * from user where user_name='%s' and password='%s'"
#define SQL_USER_CREATE "insert into user values ('%s', '%s')"
#define SQL_GAMES "select * from game where user_name='%s'"
#define SQL_ALL_GAMES "select * from game"
#define SQL_POTATO "select question_text, answer_text from question natural join answer where is_correct = true"
#define SQL_QUESTIONS "select question_id, question_text, answer_text, is_correct from answer natural join question WHERE game_id='%s'"
#define SQL_QUESTIONS_COUNT "select COUNT(question_id) from question WHERE game_id='%s'"
