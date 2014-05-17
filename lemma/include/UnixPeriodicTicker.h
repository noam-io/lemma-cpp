#ifndef UNIX_PERIODIC_TICKER_H
#define UNIX_PERIODIC_TICKER_H
#include "PeriodicTicker.h"

struct timeval;

class UnixPeriodicTicker : public PeriodicTicker
{
public:
  UnixPeriodicTicker(float milliseconds);
  virtual ~UnixPeriodicTicker();
  bool isItTime();

private:
  struct timeval * lastUpdate;
};

#endif
