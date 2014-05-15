#include "EventFilter.h"
#include "NoamServerLocator.h"
#include "MessageParser.h"
#include "MessageBuilder.h"
#include "LemmaApi.h"
#include "LemmaList.h"
#include <stdlib.h>
#include <stdio.h>
#include "TcpServer.h"
#include "TcpClient.h"
#include "UdpListener.h"


LemmaApi::LemmaApi( const char * lemmaId )
  : udpListener(0)
  , lemmaId(lemmaId)
  , connected(false)
  , maestroIpAddress(0)
  , listenPort(-1)
  , broadcastPort(-1)

//TODO: test and extract reconnect timeout
{
	reconnectTimeoutMS = 500;
	server = new TcpServer(*this);
	client = new TcpClient();
	filter = new EventFilter();
	locator = new NoamServerLocator();
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
	return lemmaId;
}

void LemmaApi::setLemmaId(const char* _lemmaId){
	lemmaId = _lemmaId;
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
		MessageBuilder builder(lemmaId);
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

void LemmaApi::begin(int broadcastPort)
{
	if(udpListener != NULL)
		free(udpListener);
	udpListener = new UdpListener();
	udpListener->startup();
	udpListener->createSocket();
	udpListener->bindTo(broadcastPort);
	server->start();
}


bool LemmaApi::messageReceived(const char* msg, size_t length)
{
  Event * e = MessageParser::parse(msg);
  filter->handle(e);
  delete e;
  return true;
}

bool LemmaApi::findMaestro()
{
  if(udpListener->attemptRead())
  {
    maestroIpAddress = udpListener->lastAddress();
    listenPort = locator->parsePort(udpListener->message);
    return (listenPort != -1);
  }
  return false;
}

bool LemmaApi::maestroLocationKnown()
{
  return (maestroIpAddress != 0 && listenPort != -1);
}

//TODO:  Test return value
bool LemmaApi::run()
{
  if (!connected && _isTimeToReconnect())
  {
    if (udpListener != 0)
    {
      findMaestro();
    }
    if (maestroLocationKnown())
    {
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
  MessageBuilder builder(lemmaId);
  char * message = builder.buildEvent(name, value);
  sendMessageToClient(message);
  // buildEvent creates new char* so the message must be freed
  //TODO: confirm this
  free(message);
}

void LemmaApi::sendEvent(char const * name, int value)
{
	//printf("Send {'%s' : %d }\n", name, value);
  MessageBuilder builder(lemmaId);
  char * message = builder.buildEvent(name, value);
  sendMessageToClient(message);
}

void LemmaApi::sendEvent(char const * name, double value)
{
  MessageBuilder builder(lemmaId);
  char * message = builder.buildEvent(name, value);
  sendMessageToClient(message);
}


void LemmaApi::sendEvent(char const * name, struct LemmaList * list)
{
  MessageBuilder builder(lemmaId);
  char * message = builder.buildEvent(name, list);
  sendMessageToClient(message);
}
