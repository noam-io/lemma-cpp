
#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <string.h>

class Connection;

class DataHandler {
public:
	virtual int handle(char* data, size_t length, Connection& conn) = 0;
};

#endif
