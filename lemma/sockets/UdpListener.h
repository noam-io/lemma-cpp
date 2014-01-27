

#ifndef UDP_LISTENER_H
#define UDP_LISTENER_H

#include "utils.h"

const int MAX_BUFFER_SIZE = 1024;

class UdpListener
{
public:
	UdpListener();
	bool startup();
	bool createSocket();
	bool bindTo(int port);
	bool attemptRead();
	bool close();
	char* lastAddress();
	int messageLength;
	char message[MAX_BUFFER_SIZE];

private:
	SOCKET RecvSocket;
	sockaddr_in SenderAddr;
	char lastAddressBuf[24];
};







#endif
