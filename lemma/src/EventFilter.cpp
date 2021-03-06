//Copyright (c) 2014, IDEO


#include "config.h"
#include "EventFilter.h"
#include "Event.h"
#include <stdlib.h>
#include <string.h>

EventFilter::EventFilter() : nFilters(0)
{
}

EventFilter::~EventFilter()
{
  for(int i = 0; i < nFilters; i++)
  {
    delete[] filters[i].eventName;
  }
}

void EventFilter::add(const char * eventName, event_handler_t callback)
{
  if (nFilters < MAX_NUM_EVENTS)
  {
    int filterNumber = nFilters++;
    filters[filterNumber].eventName = new char[strlen(eventName) + 1];
    strcpy(filters[filterNumber].eventName, eventName);
    filters[filterNumber].callback = callback;
    eventNames[filterNumber] = filters[filterNumber].eventName;
  }
}

void EventFilter::handle(Event const * event)
{
  for(int i = 0; i < nFilters; i++)
  {
    if (strcmp(event->name(), filters[i].eventName) == 0)
    {
      filters[i].callback(&event->asStruct());
    }
  }
}

int EventFilter::count()
{
  return nFilters;
}

char const** EventFilter::events()
{
  return eventNames;
}
