//Copyright (c) 2014, IDEO 

#include "../include/NoamServerLocator.h"
#include <stdio.h>

int NoamServerLocator::parsePort(char const * datagram)
{
	int port = -1;
	int matched = 0;
	if (!datagram)
		return -1;

    matched = sscanf( datagram, "[Maestro@%u]", &port);
	if (matched == 1)
		return port;
	else
		return -1;
}
