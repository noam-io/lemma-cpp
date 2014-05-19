//Copyright (c) 2014, IDEO 


#ifndef UTILS_H
#define UTILS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#define INVALID_SOCKET -1;
#define SOCKET_ERROR -1

#define NO_ERROR 0

typedef int SOCKET;

extern const char* GetLastErrorMessage(const char* pcMessagePerfix, int nErrorID = 0);
extern bool ShutdownConnection(SOCKET sd);

#endif
