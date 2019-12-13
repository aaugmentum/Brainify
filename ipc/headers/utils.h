#include <stdio.h>
#include <string.h>
#include "methods.h"
#define PORT 8080

void terminate(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
