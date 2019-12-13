#include <stdio.h>
#include <string.h>
#include "methods.h"
#define PORT 8080

void terminate(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

method_t *getMethod(int type, void *data)
{
	method_t *method = malloc(sizeof(method_t));
	method->type = type;
	switch (type)
	{
	case LOGIN:
		memcpy(method->data, data, sizeof(auth_t));
		break;
	case RESULT:
		memcpy(method->data, data, sizeof(result_t));
		break;
	default:
		break;
	}
	return method;
}