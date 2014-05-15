#ifndef MESSAGE_BUILDER_H
#define MESSAGE_BUILDER_H

struct json_t;
struct LemmaList;

class MessageBuilder
{
  private:
  char const * lemmaId;
	json_t * eventBase(const char * name) const;

  public:
  MessageBuilder( char const * lemmaId );
  char * buildEvent( char const * name, char const * value ) const;
  char * buildEvent( char const * name, int value ) const;
  char * buildEvent( char const * name, double value ) const;
  char * buildEvent( char const * name, struct LemmaList * value ) const;
  char * buildRegister( int port, char const** hears, int hearsSize, char const** plays, int playsSize ) const;
  char * buildMarco(const char* roomName);
};

#endif

