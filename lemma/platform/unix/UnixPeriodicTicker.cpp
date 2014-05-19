//Copyright (c) 2014, IDEO 

#include "UnixPeriodicTicker.h"
#include <sys/time.h>

UnixPeriodicTicker::UnixPeriodicTicker(float ms) : PeriodicTicker(ms)
{
  lastUpdate = new struct timeval;
  lastUpdate->tv_usec = 0;
  lastUpdate->tv_sec = 0;
}

UnixPeriodicTicker::~UnixPeriodicTicker()
{
  delete lastUpdate;
}

bool UnixPeriodicTicker::isItTime(){
	struct timeval tValCur;
	gettimeofday(&tValCur, NULL);

	long LastTime = lastUpdate->tv_sec * 1000000L + lastUpdate->tv_usec;
	long CurrTime = tValCur.tv_sec * 1000000L + tValCur.tv_usec;

	float elapsedMS = (CurrTime - LastTime) / 1000.0;
	if(elapsedMS >= periodMs){
		gettimeofday(lastUpdate, NULL);
		return true;
	}
	return false;
}
