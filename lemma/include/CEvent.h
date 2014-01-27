#include <string.h>


#ifndef C_EVENT
#define C_EVENT

#define EVENT_STRING_MAX_LENGTH 1024

typedef struct {
  char name[EVENT_STRING_MAX_LENGTH];
  char stringValue[EVENT_STRING_MAX_LENGTH];
  long intValue;
  double floatValue;
  char const ** list;
  size_t listLength;
} CEvent;

#endif
