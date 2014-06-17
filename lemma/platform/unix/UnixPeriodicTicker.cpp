//Copyright (c) 2014, IDEO 

#include "UnixPeriodicTicker.h"
#include <sys/time.h>

UnixPeriodicTicker::UnixPeriodicTicker(float ms) : PeriodicTicker(ms)
{
  lastUpdate = new struct timeval;
  gettimeofday(lastUpdate, 0);
}

UnixPeriodicTicker::~UnixPeriodicTicker()
{
  delete lastUpdate;
}

bool UnixPeriodicTicker::isItTime(){
	struct timeval tValCur;
	gettimeofday(&tValCur, 0);

	long LastTime = lastUpdate->tv_sec * 1000000L + lastUpdate->tv_usec;
	long CurrTime = tValCur.tv_sec * 1000000L + tValCur.tv_usec;

	long elapsedUs = (CurrTime - LastTime);
	long periodUs = (long)(periodMs * 1000L);
	if(elapsedUs >= periodUs){
		gettimeofday(lastUpdate, 0);
		return true;
	}
	return false;
}
