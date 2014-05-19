//Copyright (c) 2014, IDEO 

#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H

class Event;
class Polo;

class MessageParser
{
  public:
  static Event * parse( char const * message );
  static Polo * parsePolo( char const * message );

  private:
  MessageParser();
};

#endif

