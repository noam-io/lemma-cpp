#include <stdlib.h>
#include <stdio.h>

#include "EventFilter.h"
#include "HostLocator.h"
#include "LemmaApi.h"
#include "LemmaList.h"
#include "MessageBuilder.h"
#include "MessageParser.h"
#include "PeriodicTicker.h"
#include "TcpClient.h"
#include "TcpServer.h"
#include "UnixUdp.h"
#include "config.h"
#include "UnixPeriodicTicker.h"

LemmaApi::LemmaApi( const char * guestName, const char * desiredRoomName)
  : udpListener(0)
  , guestName(guestName)
  , desiredRoomName(desiredRoomName)
  , connected(false)
  , maestroIpAddress(0)
  , listenPort(-1)

//TODO: test and extract reconnect timeout
{
	reconnectTimeoutMS = 500;
	server = new TcpServer(*this);
	client = new TcpClient();
	filter = new EventFilter();
  marcoTicker = new UnixPeriodicTicker(2008);
	gettimeofday(&lastUpdate, NULL);
}


LemmaApi::~LemmaApi()
{
  delete filter;
  delete client;
  delete server;
  if (udpListener != 0)
    delete udpListener;
  delete locator;
}

const char* LemmaApi::getLemmaId(){
	return guestName;
}

void LemmaApi::setLemmaId(const char* _guestName){
	guestName = _guestName;
}

void LemmaApi::hear(const char* eventName, event_handler_t handler)
{
  filter->add(eventName, handler);
}

bool LemmaApi::isConnected(){
	return connected;
}

void LemmaApi::sendMessageToClient(const char * message)
{
  if (!connected)
    return;
  int result = 0;
  char length[7];
  snprintf(length, 7, "%06d", (int)strlen(message));
  result += client->sendMessage(length, 6);
  result += client->sendMessage(message, strlen(message));
  if (result != 0)
    connected = false;
}

bool LemmaApi::connectAndRegister()
{
	if( 0 == client->connect(maestroIpAddress, listenPort))
	{
		connected = true;
		MessageBuilder builder(guestName);
		char * message = builder.buildRegister(server->listeningPort(), filter->events(), filter->count(), 0, 0);
		sendMessageToClient(message);
		free(message);
		return true;
	}
	return false;
}

void LemmaApi::begin(const char * _maestroIpAddress, int maestroPort)
{
  maestroIpAddress = _maestroIpAddress;
  listenPort = maestroPort;
  server->start();
}

void LemmaApi::begin()
{
	if(udpListener != NULL)
		free(udpListener);
	udpListener = new UnixUdp(BROADCAST_SEND_PORT);
	udpListener->createSocket();
	udpListener->bind();

	locator = new HostLocator(*udpListener, *marcoTicker, guestName, desiredRoomName);

	server->start();
}


bool LemmaApi::messageReceived(const char* msg, size_t length)
{
  Event * e = MessageParser::parse(msg);
  filter->handle(e);
  delete e;
  return true;
}

//TODO:  Test return value
bool LemmaApi::run()
{
  if (!connected && _isTimeToReconnect())
  {
    locator->tryLocate();
    if (locator->isFound())
    {
      maestroIpAddress = locator->ipAddress();
      listenPort = locator->port();
      connectAndRegister();
    }
  }

  if(connected){
	  server->run();
  }

  return connected;
}

//TODO: extract and test
bool LemmaApi::_isTimeToReconnect(){
	struct timeval tValCur;
	gettimeofday(&tValCur, NULL);

	long LastTime = lastUpdate.tv_sec * 1000000L + lastUpdate.tv_usec;
	long CurrTime = tValCur.tv_sec * 1000000L + tValCur.tv_usec;

	float elapsedMS = (CurrTime - LastTime) / 1000.0;
	if(elapsedMS >= reconnectTimeoutMS){
		gettimeofday(&lastUpdate, NULL);
		return true;
	}
	return false;
}

void LemmaApi::sendEvent(char const * name, const char * value)
{
  MessageBuilder builder(guestName);
  char * message = builder.buildEvent(name, value);
  sendMessageToClient(message);
  // buildEvent creates new char* so the message must be freed
  //TODO: confirm this
  free(message);
}

void LemmaApi::sendEvent(char const * name, int value)
{
	//printf("Send {'%s' : %d }\n", name, value);
  MessageBuilder builder(guestName);
  char * message = builder.buildEvent(name, value);
  sendMessageToClient(message);
}

void LemmaApi::sendEvent(char const * name, double value)
{
  MessageBuilder builder(guestName);
  char * message = builder.buildEvent(name, value);
  sendMessageToClient(message);
}


void LemmaApi::sendEvent(char const * name, struct LemmaList * list)
{
  MessageBuilder builder(guestName);
  char * message = builder.buildEvent(name, list);
  sendMessageToClient(message);
}
