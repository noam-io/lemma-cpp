#ifndef POLO_H
#define POLO_H
class Polo
{
public:
  Polo();
  ~Polo();
  void setRoomName(const char* _roomName);
  char * roomName;
  int port;
};
#endif
