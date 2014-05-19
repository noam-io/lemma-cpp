//Copyright (c) 2014, IDEO 

#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include "utils.h"

class TcpClient
{
private:
	int mSocket;

public:
	TcpClient();
	virtual ~TcpClient();

	int connect();
	int connect(const char* pcHost, int nPort);

	int sendMessage(const char* message, size_t messageLength);
	int read(char* buffer, size_t size);
};

#endif
