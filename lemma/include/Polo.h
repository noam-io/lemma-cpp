//Copyright (c) 2014, IDEO 

#ifndef POLO_H
#define POLO_H

#define POLO_MAX_STRING_LEN 128
class Polo
{
public:
  Polo();
  ~Polo();
  void setRoomName(const char* _roomName);
  char roomName[POLO_MAX_STRING_LEN];
  int port;
};
#endif
