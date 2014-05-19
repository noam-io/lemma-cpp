//Copyright (c) 2014, IDEO 

#ifndef UDP_LISTENER_H
#define UDP_LISTENER_H

#include "utils.h"
#include "Udp.h"

class UnixUdp : public Udp
{
public:
	UnixUdp( int broadcastPort );
	bool createSocket();
	bool bind(int port = 0);
	virtual bool attemptRead();
	virtual bool close();
	virtual char* lastAddress();
  virtual bool broadcast(const char * message);

private:
	SOCKET RecvSocket;
	sockaddr_in SenderAddr;
  int broadcastPort;
};







#endif
