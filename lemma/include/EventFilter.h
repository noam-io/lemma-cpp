//Copyright (c) 2014, IDEO 

#ifndef EVENT_FILTER_H
#define EVENT_FILTER_H

#define MAX_NUM_EVENTS 64
#include "config.h"
#include "EventHandler.h"

class Event;

class EventFilter
{
public:
  EventFilter();
  ~EventFilter();

  void add(const char * eventName, event_handler_t callback);
  void handle(Event const *);
  int count();
  char const** events();

private:
  char const* eventNames[MAX_NUM_EVENTS];

  typedef struct {
    char * eventName;
    event_handler_t callback;
  } filter_t;

  filter_t filters[MAX_NUM_EVENTS];
  int nFilters;
};

#endif

