//Copyright (c) 2014, IDEO 

#include <string.h>
#include <stdlib.h>
#include "Polo.h"
#include <stdio.h>

Polo::Polo() {
	this->roomName[0] = '\0';
}

Polo::~Polo() {
}

void Polo::setRoomName(const char* _roomName)
{ 
  strncpy(this->roomName, _roomName, POLO_MAX_STRING_LEN);
  this->roomName[strlen(_roomName)] = '\0';
}
