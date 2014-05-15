#include <string.h>

#include "HostLocator.h"
#include "MessageBuilder.h"
#include "MessageParser.h"
#include "Polo.h"
#include "Udp.h"


HostLocator::HostLocator(Udp& _udp, const char * lemmaId, const char * roomName)
   : roomName(roomName)
   , lemmaId(lemmaId)
   , udp(_udp)
   , found(false)
   , hostPort(-1)
{
  memset(hostIpAddress, 0, 24);
}

void HostLocator::begin()
{
}

void HostLocator::tryLocate()
{
  MessageBuilder messageBuilder(lemmaId);
  udp.broadcast(messageBuilder.buildMarco(roomName));

  if(udp.attemptRead())
  {
    Polo* polo = MessageParser::parsePolo(udp.message);
    if (polo)
    {
      found = true;
      strncpy(hostIpAddress, udp.lastAddress(), 24);
      hostPort = polo->port;
      delete polo;
    }
  }
}

bool HostLocator::isFound()
{
  return found;
}

const char * HostLocator::ipAddress()
{
  return hostIpAddress;
}

int HostLocator::port()
{
  return hostPort;
}
