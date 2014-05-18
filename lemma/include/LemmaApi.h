#ifndef LEMMA_API_H
#define LEMMA_API_H

#include "Event.h"
#include "TcpReader.h"
#include "EventHandler.h"
#include <sys/time.h>

class TcpClient;
class TcpServer;
class EventFilter;
class UnixUdp;
class HostLocator;
class PeriodicTicker;

struct LemmaList;

class LemmaApi : public TcpReader
{
public:
  LemmaApi( const char * guestName, const char * desiredRoomName = "" );
  virtual ~LemmaApi();

  void hear(const char * eventName, event_handler_t handler);
  void begin(const char* maestroIpAddress, int maestroPort);
  void begin();
  bool run(); //poll in program loop
  void speak(char const * name, const char * value);
  void speak(char const * name, int value);
  void speak(char const * name, double value);
  void speak(char const * name, struct LemmaList * list);
  const char* getLemmaId();
  void setLemmaId(const char* _lemmaId);

  bool isConnected();
  virtual bool messageReceived(const char* msg, size_t length);
  void sendMessageToClient(const char * message);

private:
  LemmaApi & operator=(LemmaApi const &);
  LemmaApi(LemmaApi const &);
  EventFilter * filter;
  TcpServer * server;
  TcpClient * client;
  UnixUdp * udpListener;
  PeriodicTicker * marcoTicker;
  PeriodicTicker * reconnectTicker;
  HostLocator * locator;
  const char* guestName;
  const char* desiredRoomName;

  bool connected;
  bool connectAndRegister();
  const char* maestroIpAddress;
  int listenPort;

};


#endif
