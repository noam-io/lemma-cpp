#include <string.h>

#include "HostLocator.h"
#include "MessageBuilder.h"
#include "MessageParser.h"
#include "PeriodicTicker.h"
#include "Polo.h"
#include "Udp.h"


HostLocator::HostLocator( Udp& _udp, PeriodicTicker & ticker, const char * lemmaId, const char * roomName )
   : roomName( roomName )
   , lemmaId( lemmaId )
   , udp( _udp )
   , found( false )
   , hostPort( -1 )
   , ticker( ticker )
{
  memset(hostIpAddress, 0, 24);
}

void HostLocator::begin()
{
}

#include <stdio.h>
void HostLocator::tryLocate()
{
  if ( ticker.isItTime() )
  {
    printf("Broadcasting\n");
    MessageBuilder messageBuilder(lemmaId);
    udp.broadcast(messageBuilder.buildMarco(roomName));
  }

  if(udp.attemptRead())
  {
    printf("Rxed ");
    printf("%s", udp.message);
    printf("\n");
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
