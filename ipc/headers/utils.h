#pragma once
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "methods.h"
#include "consts.h"

//!Terminate with msg
void terminate(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

//*Try to send all packages
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

//!Exit if server disconnects
int my_recv(int fd, void *buf, int n, int flags)
{
	int size = recv(fd, buf, n, flags);
	if (size < 1)
		terminate("Disconnected from server");
	return size;
}

//*Generate pin
int generate_luminous_element()
{
	int r = rand() % 100000;
	return r;
}