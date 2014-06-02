//Copyright (c) 2014, IDEO

#include "LemmaList.h"
#include "LemmaListSerializer.h"
#include "LemmaUtil.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "jansson.h"


json_t * LemmaList_Jsonify(LemmaList * self)
{
	json_t *ary = json_array();
	LemmaList * subList;
    int listLength = LemmaList_GetLength(self);
    int i;

    for(i = 0; i < listLength; i++)
    {
		subList = LemmaList_GetListAt(self, i);
		if ( subList != 0 )
		{
			json_array_append_new(ary, LemmaList_Jsonify(subList));
		}
		else
		{
			json_array_append_new(ary, json_string(LemmaList_GetStringAt(self, i)));
		}
	}
	return ary;
}

char* LemmaList_Serialize(LemmaList* self)
{
	return json_dumps(LemmaList_Jsonify(self), 0);
}

void LemmaList_Release(char *serializedResults)
{
  if(serializedResults)
    jsonp_free(serializedResults);
}
