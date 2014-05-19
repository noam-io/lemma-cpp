//Copyright (c) 2014, IDEO 

#include <string.h>
#include "Polo.h"

Polo::Polo() : roomName(0) {}

Polo::~Polo() {
  if(roomName)
    delete[] roomName;
}

void Polo::setRoomName(const char* _roomName)
{
  size_t length = strlen(_roomName) + 1;
  roomName = new char[length];
  strncpy(roomName, _roomName, length);
}
