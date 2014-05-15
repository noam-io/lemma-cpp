#include "HostLocator.h"
#include "MessageBuilder.h"
#include "Udp.h"

HostLocator::HostLocator(Udp& _udp, const char * lemmaId, const char * roomName)
   : roomName(roomName)
   , lemmaId(lemmaId)
   , udp(_udp)
   , found(false)
{
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
    found = true;
  }
}

bool HostLocator::isFound()
{
  return found;
}
