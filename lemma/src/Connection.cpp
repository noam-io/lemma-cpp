//Copyright (c) 2014, IDEO 


#include "Connection.h"
#include <string.h>

Connection::Connection(int sd) : sd(sd),
	nCharsInBuffer(0),
	nCharsInWriteBuffer(0)
{

}

int Connection::write(char* data, size_t length)
{
	if(nCharsInWriteBuffer + length > kBufferSize)
	{
		return 0;
	}

	memcpy(&writeBuffer[nCharsInWriteBuffer], data, length);
	nCharsInWriteBuffer += length;
	return length;
}
