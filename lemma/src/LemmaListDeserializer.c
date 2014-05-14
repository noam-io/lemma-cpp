#include "LemmaList.h"
#include <stdlib.h>
#include <string.h>
#include "jansson.h"

LemmaList* LemmaList_Dejsonify(json_t* json)
{
        int listLength;
		int i;
        LemmaList * list = 0;

        list = LemmaList_Create();

        listLength = json_array_size(json);
        
		for(i = 0; i < listLength; i++)
        {
			json_t * subElement = json_array_get(json, i);
			if ( json_is_array(subElement) )
			{
				LemmaList_AddList(list, LemmaList_Dejsonify(subElement));
			}
			else
			{
                LemmaList_AddString(list, json_string_value(subElement));
      		}
		}
        return list;

}

LemmaList* LemmaList_Deserialize(char const* serializedList)
{
	json_error_t error;
	json_t* json;

	if(serializedList == 0 || strlen(serializedList) == 0)
        return 0;

	json = json_loads(serializedList, JSON_DISABLE_EOF_CHECK, &error);
	if( json == 0 )
		return 0;

	return LemmaList_Dejsonify(json);
}

