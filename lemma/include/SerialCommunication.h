//Copyright (c) 2014, IDEO

/*
 * SerialCommunication.h
 *
 *  Created on: Feb 4, 2014
 *      Author: dbrody
 */

#ifndef SERIALCOMMUNICATION_H_
#define SERIALCOMMUNICATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

class SerialCommunication {
public:

	bool isOpen;
	int fd;
	char device[64];
	speed_t baud;

	// TTY Attributes
	// Store old attributes to restore them at end
	struct termios oldkey;
	struct termios tty;

	SerialCommunication(const char* device, speed_t baud = B19200);
	bool start();
	virtual ~SerialCommunication();

	ssize_t readMessage(int len, char* buffer);
	ssize_t writeMessage(int len, const char* buffer);
};

#endif /* SERIALCOMMUNICATION_H_ */
