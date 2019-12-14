#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "methods.h"
#define PORT 8080

void terminate(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

int sendall(int fd, void *buf, int n, int flags)
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

int generate_luminous_element()
{
	srand(time(NULL)); // Initialization, should only be called once.
	int r = rand() % 100000;
	return r;
}