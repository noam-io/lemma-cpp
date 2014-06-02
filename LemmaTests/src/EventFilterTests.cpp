//Copyright (c) 2014, IDEO

#include "event.h"
#include "EventFilter.h"
#include "CppUTest/TestHarness.h"

  static CEvent const * event;

  void testHandler(CEvent const * e)
  {
   event = e;
  }

TEST_GROUP(EventFilter)
{
};

TEST(EventFilter, singleRegistration) {
  EventFilter filter;
  filter.add("speed", testHandler);
  Event speed("speed", "12");

  filter.handle( &speed );
  STRCMP_EQUAL(speed.name(), event->name );
  STRCMP_EQUAL(speed.stringValue(), event->stringValue );
}

TEST(EventFilter, multipleRegistrations) {
  EventFilter filter;
  filter.add("speed", testHandler);
  filter.add("rpms", testHandler);

  Event speed("speed", "12");
  Event rpms("rpms", "3500");

  filter.handle( &speed );
  STRCMP_EQUAL( speed.name(), event->name );
  STRCMP_EQUAL( speed.stringValue(), event->stringValue );

  filter.handle( &rpms );
  STRCMP_EQUAL( rpms.name(), event->name );
  STRCMP_EQUAL( rpms.stringValue(), event->stringValue );
}

TEST(EventFilter, getsListOfEvents) {
  EventFilter filter;
  filter.add("speed", testHandler);
  filter.add("rpms", testHandler);
  CHECK_EQUAL(2, filter.count());
  char const** events = filter.events();
  STRCMP_EQUAL( events[0], "speed" );
  STRCMP_EQUAL( events[1], "rpms" );
}

TEST(EventFilter, maxesOutFilters) {
  EventFilter filter;
  for( int i = 0; i < MAX_NUM_EVENTS; i++ )
  {
    filter.add("speed", testHandler);
  }

  filter.add("speed65", testHandler);
  CHECK_EQUAL(MAX_NUM_EVENTS, filter.count());
}

