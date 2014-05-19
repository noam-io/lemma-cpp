//Copyright (c) 2014, IDEO 


#ifndef CONNECTION_H
#define CONNECTION_H

#include <string.h>


const int kBufferSize = 1024 * 512;
class Connection {

public:
	int sd;
	char readBuffer[kBufferSize];
	int nCharsInBuffer;
	char writeBuffer[kBufferSize];
	int nCharsInWriteBuffer;

	Connection( int sd_ = 0);
	int write(char* data, size_t length);


};

#endif
