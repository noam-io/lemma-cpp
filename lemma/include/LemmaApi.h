#ifndef LEMMA_API_H
#define LEMMA_API_H

#include "Event.h"
#include "../sockets/TcpReader.h"
#include "EventHandler.h"
#include <sys/time.h>

class TcpClient;
class TcpServer;
class EventFilter;
class UdpListener;
class NoamServerLocator;

struct LemmaList;

class LemmaApi : public TcpReader
{
public:
  LemmaApi( const char * lemmaId );
  virtual ~LemmaApi();

  void hear(char * eventName, event_handler_t handler);
  void begin(char* maestroIpAddress, int maestroPort);
  void begin(int broadcastPort);
  bool run(); //poll in program loop
  void sendEvent(char const * name, char* value);
  void sendEvent(char const * name, int value);
  void sendEvent(char const * name, double value);
  void sendEvent(char const * name, struct LemmaList * list);
  bool isConnected();
  const char* getLemmaId();
  void setLemmaId(const char* _lemmaId);
  virtual bool messageReceived(char* msg, size_t length);
  void sendMessageToClient(const char * message);

private:
  LemmaApi & operator=(LemmaApi const &);
  LemmaApi(LemmaApi const &);
  bool _isTimeToReconnect();
  int reconnectTimeoutMS;
  EventFilter * filter;
  TcpServer * server;
  TcpClient * client;
  UdpListener * udpListener;
  NoamServerLocator * locator;
  struct timeval lastUpdate;
  const char* lemmaId;

  bool connected;
  bool connectAndRegister();
  char* maestroIpAddress; 
  int listenPort;
  int broadcastPort;
  bool findMaestro();
  bool maestroLocationKnown();


};


#endif
