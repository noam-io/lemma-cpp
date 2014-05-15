#ifndef UDP_LISTENER_H
#define UDP_LISTENER_H

#include "utils.h"
#include "Udp.h"

class UdpListener : public Udp
{
public:
	UdpListener();
	bool startup();
	bool createSocket();
	bool bindTo(int port);
	virtual bool attemptRead();
	virtual bool close();
	virtual char* lastAddress();
  virtual bool broadcast(const char * message);

private:
	SOCKET RecvSocket;
	sockaddr_in SenderAddr;
	char lastAddressBuf[24];
};







#endif
