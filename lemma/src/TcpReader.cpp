#include "TcpReader.h"
#include <string.h>
#include <stdlib.h>

static const int SIZE_OF_LENGTH_FIELD = 6;

static int payloadLength(const char * data, int& bytesRemaining)
{
  int payloadSize = -1;
  if ( bytesRemaining >= SIZE_OF_LENGTH_FIELD ){
    char lengthBuffer[SIZE_OF_LENGTH_FIELD + 1];
    memcpy(lengthBuffer, data, SIZE_OF_LENGTH_FIELD);
    lengthBuffer[SIZE_OF_LENGTH_FIELD] = 0;
    payloadSize = atoi(lengthBuffer);
    bytesRemaining -= SIZE_OF_LENGTH_FIELD;
  }
  return payloadSize;
}

static bool payloadIsReady(int payloadSize, int bytesRemaining)
{
  return (payloadSize > 0 && payloadSize <= bytesRemaining);
}

int TcpReader::handle(const char * data, size_t length, Connection& )
{
  int bytesRemaining = (int)length;
  int payloadSize = payloadLength( data, bytesRemaining );
  int bytesHandled = 0;
  while( payloadIsReady(payloadSize, bytesRemaining )) {
    this->messageReceived( &data[ length - bytesRemaining ], payloadSize );
    bytesRemaining -= payloadSize;
    bytesHandled += payloadSize + SIZE_OF_LENGTH_FIELD;
    payloadSize = payloadLength( &data[ length - bytesRemaining ], bytesRemaining );
  }
  return bytesHandled;
}
