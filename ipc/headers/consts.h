#define DB_HOST "remotemysql.com"
#define DB_USER "71VUvOSzmm"
#define DB_PASSWD "rpraRefD0g"
#define DB_DB "71VUvOSzmm"
#define DB_PORT 3306
#define PORT 8080

//SQL queries 
#define SQL_USER_EXIST "select * from user where user_name='%s' and password='%s'"
#define SQL_GAMES "select * from game where user_name='%s'"
#define SQL_POTATO "select question_text, answer_text from question natural join answer where is_correct = true"
