//Copyright (c) 2014, IDEO

#ifndef EVENT_H
#define EVENT_H

#include "config.h"
#include "CEvent.h"

class Event {
public:
  char const* name() const;
  char const* stringValue() const;
  long intValue() const;
  double floatValue() const;
  int listLength() const;
  char const ** list() const;

  Event();
  virtual ~Event();
  Event( char const * name, char const * value );
  Event( char const * name, long value );
  Event( char const * name, double value );
  CEvent const & asStruct() const;


private:
  Event & operator=(Event const & other);
  Event( Event const & other);

protected:
  CEvent e;

};

class EventList : public Event
{
public:
  EventList( char const * name, size_t listLength );
  virtual ~EventList();
  void set( size_t index, char const * value );
  char const * get( size_t index ) const;
};

#endif
