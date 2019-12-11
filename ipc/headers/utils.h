#include <stdlib.h>
#include <stdio.h>
#define PORT 8080

void terminate(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}