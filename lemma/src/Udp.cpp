//Copyright (c) 2014, IDEO 

#include <string.h>

#include "Udp.h"

Udp::Udp()
 : messageLength(0)
{
  memset(message, 0, MAX_BUFFER_SIZE);
}

Udp::~Udp()
{
}
