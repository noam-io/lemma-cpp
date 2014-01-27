#include "../include/MessageParser.h"
#include "../include/Event.h"
#include "../jansson/jansson.h"

Event * MessageParser::parse( char const * message )
{
  json_error_t error;
  json_t * eventName;
  json_t * eventValue;
  json_t * json = json_loads(message, JSON_DISABLE_EOF_CHECK, &error);
  if(json) 
  {
    eventName = json_array_get( json, 2 );
    eventValue = json_array_get( json, 3 );
    size_t size = 0;
    Event * result = 0;
    switch(json_typeof(eventValue))
    {
      case JSON_INTEGER:
        {
          result = new Event( json_string_value(eventName), (long)json_integer_value(eventValue) );
          break;
        }

      case JSON_REAL:
        {
          result = new Event( json_string_value(eventName), json_real_value(eventValue) );
          break;
        }
      case JSON_ARRAY:
        {
          size = json_array_size(eventValue);

          EventList * resultList = new EventList( json_string_value(eventName), size);
          for (size_t i = 0; i< size; i++)
          {
            resultList->set(i, json_string_value(json_array_get(eventValue, i)));
          }
          result = resultList;
          break;
        }
      default:
        {
          result = new Event( json_string_value(eventName), json_string_value(eventValue) );
          break;
        }
    }
    json_decref( json );
    return result;
  }
  return new Event();
}

