#ifndef PERIODIC_TICKER_H
#define PERIODIC_TICKER_H
class PeriodicTicker
{
public:
  PeriodicTicker(float milliseconds);
  virtual ~PeriodicTicker();
  virtual bool isItTime() = 0;

protected:
  float periodMs;
};

#endif
