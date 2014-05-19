//Copyright (c) 2014, IDEO 

/*
 * SerialCommunication.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: dbrody
 */

#include "SerialCommunication.h"

SerialCommunication::SerialCommunication(const char* _device, speed_t _baud) {
	strcpy(device, _device);
	baud = _baud;
	isOpen = false;
	fd = 0;

	// Reset TTY Attribute structs
	memset(&oldkey, 0, sizeof oldkey);
	memset(&tty, 0, sizeof tty);
}

bool SerialCommunication::start(){
	// Open Connection To Serial Device
	fd = open(device, O_RDWR | O_NONBLOCK | O_NOCTTY);
	if(fd < 0){
		return false;
	}

	// Set Serial Device Properties
	fcntl(fd, F_SETFL, 0);
	tcgetattr(fd, &oldkey);
	if(tcgetattr(fd, &tty) != 0){
		std::cout << "Error getting tty attributes." << std::endl;
		return false;
	}

	cfsetospeed(&tty, baud);
	cfsetispeed(&tty, baud);

	tty.c_cflag		&= ~PARENB;
	tty.c_cflag		&= ~CSTOPB;
	tty.c_cflag		&= ~CSIZE;
	tty.c_cflag		|= CS8;
	tty.c_cflag		&= ~CRTSCTS;
	tty.c_lflag		= 0;
	tty.c_oflag		= 0;
	tty.c_cc[VMIN]	= 0;
	tty.c_cc[VTIME]	= 0;

	tty.c_cflag		|= CREAD | CLOCAL;
	tty.c_iflag		&= ~(IXON | IXOFF | IXANY);
	tty.c_iflag		&= ~(ICANON | ECHO | ECHOE | ISIG);
	tty.c_oflag		&= OPOST;

	tcflush(fd, TCIFLUSH);

	if(tcsetattr(fd, TCSANOW, &tty) != 0){
		std::cout << "Error setting tty attributes." << std::endl;
		return false;
	}
	isOpen = true;
	return true;
}

ssize_t SerialCommunication::readMessage(int len, char* buffer){
	if(!isOpen){
		return 0;
	}
	return read(fd, buffer, len);
}

ssize_t SerialCommunication::writeMessage(int len, const char* buffer){
	if(!isOpen){
		return 0;
	}
	return write(fd, buffer, len);
}

SerialCommunication::~SerialCommunication() {
	// Restore TTY settings
	if(fd > 0){
		tcsetattr(fd, TCSANOW, &oldkey);
		close(fd);
	}
	isOpen = false;
}

