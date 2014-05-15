#ifndef TCPREADER_H
#define TCPREADER_H

#include "DataHandler.h"

class TcpReader : public DataHandler
{
public:
	int handle( const char* data, size_t length, Connection& conn);
	virtual bool messageReceived(const char* msg, size_t length) = 0;
};

#endif
