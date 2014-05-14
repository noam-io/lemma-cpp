#include "MessageParser.h"
#include "event.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP(MessageParser) {};

TEST(MessageParser, parsingEventMessage) {
  const char * message = "[\"event\",\"some id\",\"a name\",\"a value\"]";
  const Event* event = MessageParser::parse( message );

  STRCMP_EQUAL( "a name",  event->name() );

  STRCMP_EQUAL( "a value", event->stringValue() );
  CHECK( 0 == event->intValue() );
  CHECK( 0 == event->floatValue() );
  delete event;
}

TEST(MessageParser, parsingIntValue) {
  const char * message = "[\"event\",\"some id\",\"a name\",142]";
  const Event * event = MessageParser::parse( message );

  STRCMP_EQUAL( "a name",  event->name() );
  STRCMP_EQUAL(  "142", event->stringValue() );
  CHECK( 142 == event->intValue() );
  CHECK( 142.0f == event->floatValue() );
  delete event;
}

TEST(MessageParser, parsingFloatValue) {
  const char * message = "[\"event\",\"some id\",\"a name\",14.10]";
  Event* event = MessageParser::parse( message );

  STRCMP_EQUAL( "a name",  event->name() );
  CHECK( 14 == event->intValue() );
  CHECK(abs(event->floatValue() - 14.10) < 0.01f );
  delete event;
}

TEST(MessageParser, parsesWithExtraDataHangingAround)
{
  const char * message = "[\"event\",\"some id\",\"a name\",\"a value\"]000003";
  Event * event = MessageParser::parse( message );

  STRCMP_EQUAL( "a name",  event->name() );

  STRCMP_EQUAL( "a value", event->stringValue() );
  delete event;
}

TEST(MessageParser, parsesListOfStrings)
{
  const char * message = "[\"event\",\"some id\",\"a name\",[\"a value 1\", \"a value 2\", \"a value 3\", \"a value 4\"]]";
  Event * event = MessageParser::parse( message );
  CEvent const * c_event = &event->asStruct();
  CHECK_EQUAL( (int)4, (int)c_event->listLength );
  STRCMP_EQUAL( "a value 1", c_event->list[0] );
  STRCMP_EQUAL( "a value 2", c_event->list[1] );
  STRCMP_EQUAL( "a value 3", c_event->list[2] );
  STRCMP_EQUAL( "a value 4", c_event->list[3] );
  delete event;
}

