#include "MessageBuilder.h"
#include "jansson.h"
extern "C" {
#include "LemmaListSerializer.h"
}

MessageBuilder::MessageBuilder( char const * id) :
  lemmaId(id)
{
}

json_t * MessageBuilder::eventBase(const char* name) const {
	json_t *array = json_array();
	json_array_append_new(array, json_string("event"));
	json_array_append_new(array, json_string(lemmaId));
	json_array_append_new(array, json_string(name));
	return array;
}

char * MessageBuilder::buildEvent(  char const * name, char const * value ) const
{
	json_t *array = eventBase(name);
	json_array_append_new(array, json_string(value));
  char * string = json_dumps(array, 0);
	json_decref(array);
	return string;
}

char * MessageBuilder::buildEvent(  char const * name, int value ) const
{
	json_t *array = eventBase(name);
	json_array_append_new(array, json_integer(value));
    char * string = json_dumps(array, 0);
	json_decref(array);
	return string;
}

char * MessageBuilder::buildEvent(  char const * name, double value ) const
{
	json_t *array = eventBase(name);
	json_array_append_new(array, json_real(value));
    char * string = json_dumps(array, 0);
	json_decref(array);
	return string;
}

char * MessageBuilder::buildEvent(  char const * name, struct LemmaList * value ) const
{
	json_t *array = eventBase(name);
	json_array_append_new(array, LemmaList_Jsonify(value));
    char * string = json_dumps(array, 0);
	json_decref(array);
	return string;
}

void string_array_append_new(json_t * root, char const** stringArray, int size){
	int i = 0;
	json_t * array = json_array();

	for(i = 0; i < size; i++)
	{
		json_array_append_new(array, json_string(stringArray[i]));
	}
	json_array_append_new(root, array);
}


char * MessageBuilder::buildRegister( int port, char const** hears, int hearsSize, char const** plays, int playsSize ) const
{
	json_t *array = json_array();
	json_array_append_new(array, json_string("register"));
	json_array_append_new(array, json_string(lemmaId));
	json_array_append_new(array, json_integer(port));
  string_array_append_new(array, hears, hearsSize);
  string_array_append_new(array, plays, playsSize);

  char * string = json_dumps(array, 0);
	json_decref(array);
	return string;
}

