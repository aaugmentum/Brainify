#include <stdio.h>
#define PORT 8080

void terminate(const char* msg) {
	perror(msg);
	exit(0);
}

struct method
{
	int type;
	char data[1024];
};

struct login
{
	char username[50];
};

