#ifndef TEST_TICKER_H
#define TEST_TICKER_H

#include "PeriodicTicker.h"
class TestTicker : public PeriodicTicker
{

public:
  TestTicker() : PeriodicTicker(1000), time(true){};
  bool isItTime() { return  time; };
  bool time;

};

#endif
