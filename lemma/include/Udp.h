//Copyright (c) 2014, IDEO 

#ifndef UDP_H
#define UDP_H
const int MAX_BUFFER_SIZE = 1024;

class Udp
{
public:
	Udp();
  virtual ~Udp();
	virtual bool attemptRead() = 0;
	virtual const char* lastAddress() = 0;
	virtual bool close() = 0;
  virtual bool broadcast(const char * message) = 0;
	int messageLength;
	char message[MAX_BUFFER_SIZE];

};

#endif
