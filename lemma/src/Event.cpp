//Copyright (c) 2014, IDEO

#include "Event.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Event::Event()
{
  this->e.name[0] = 0;
  this->e.stringValue[0] = 0;
  this->e.intValue = 0;
  this->e.floatValue = 0.0;
  this->e.listLength = 0;
  this->e.list = 0;
}

Event::Event( char const * name, char const * value )
{
  strncpy( this->e.name, name, EVENT_STRING_MAX_LENGTH );
  strncpy( this->e.stringValue, value, EVENT_STRING_MAX_LENGTH );
  this->e.intValue = atoi( value );
  this->e.floatValue = atof( value );
}

Event::Event( char const * name, long value )
{
  strncpy( this->e.name, name, EVENT_STRING_MAX_LENGTH );
  snprintf( this->e.stringValue, EVENT_STRING_MAX_LENGTH, "%ld", value );
  this->e.intValue = value;
  this->e.floatValue = (double) value;
}

Event::Event( char const * name, double value )
{
  strncpy( this->e.name, name, EVENT_STRING_MAX_LENGTH );
  this->e.stringValue[0] = 0;
  this->e.intValue = (int) value;
  this->e.floatValue =  value;
}

Event::Event( Event const & other)
{
}

Event::~Event()
{
};

Event & Event::operator=(Event const & other)
{
  return *this;
}

char const* Event::name() const
{
  return e.name;
}

char const* Event::stringValue() const
{
  return e.stringValue;
}

long Event::intValue() const
{
  return e.intValue;
}

double Event::floatValue() const
{
  return e.floatValue;
}

int Event::listLength() const
{
  return e.listLength;
}

char const ** Event::list() const
{
  return e.list;
}

CEvent const& Event::asStruct() const
{
  return e;
}



EventList::EventList(char const* name, size_t listLength)
  : Event(name, (long)listLength)
{
  e.listLength = listLength;
  e.list = (const char **)new char*[listLength];
}

EventList::~EventList()
{
  for ( size_t i = 0; i < e.listLength; i++ )
  {
	  delete[] e.list[i];
  }
  delete[] e.list;
}

void EventList::set(size_t index, char const * value)
{
  char * newString = new char[strlen(value) + 1];
  strcpy(newString, value);
  if(e.list[index] != NULL){
	  delete[] e.list[index];
  }
  e.list[index] = newString;
}

char const * EventList::get(size_t index) const
{
  return "";
}
