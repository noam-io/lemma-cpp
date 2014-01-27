#ifndef TCPREADER_H
#define TCPREADER_H

#include "DataHandler.h"

class TcpReader : public DataHandler
{
public:
	int handle( char* data, size_t length, Connection& conn);
	virtual bool messageReceived(char* msg, size_t length) = 0;
};

#endif
