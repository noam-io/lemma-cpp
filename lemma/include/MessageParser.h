#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H

class Event;

class MessageParser
{
  public:
  static Event * parse( char const * message );

  private:
  MessageParser();
};

#endif

